#include <iostream>
//#include <atomic>
#include <thread>
#include <opencv2/opencv.hpp>

template<int SIZE>
class NoWaitBuffer {
private:
    uchar* data;
    cv::Mat buffer[SIZE];
    std::atomic<int> buffer_status[SIZE];//-1正在写/0空闲/1有一个读者/2有两个读者/...
    int sequence[SIZE];
    std::atomic<bool> sequence_lock;//TRUE就是上锁，false就是开锁
    std::atomic<bool> newdata_flags[SIZE - 2];//服务于所有读者的一组标志
public:
    NoWaitBuffer(int rows, int cols, int type) :sequence_lock(false)
    {
        data = new uchar[SIZE * rows * cols * CV_ELEM_SIZE(type)];
        for (int i = 0; i < SIZE; ++i)
        {
            buffer[i] = cv::Mat(rows, cols, type, &data[i * rows * cols * CV_ELEM_SIZE(type)]);
            buffer_status[i] = 0;
            sequence[i] = i;
        }

        for (int i = 0; i < SIZE - 2; i++)
        {
            newdata_flags[i] = false;
        }

    }
    void update(const cv::Mat& data) {
        while (sequence_lock.exchange(true));
        int update_idx = -1;//将要找一个最老旧，且可用的位置更新其数据
        int i_s = -1;//记录update_idx在顺序表中什么位置找到的
        for (int i = SIZE - 1; i >= 0; i--)
        {
            if (buffer_status[sequence[i]] == 0)
            {
                update_idx = sequence[i];
                i_s = i;
                break;
            }
        }
        if (update_idx == -1)return;
        //同时更新顺序表，将这个被修改的位置作为最新数据的位置,将这个update_idx移动到首位，中间的数据全部后移一位，直到补全原来的空隙。
        for (int i = i_s; i > 0; i--)
        {
            sequence[i] = sequence[i - 1];
        }sequence[0] = update_idx;

        buffer_status[update_idx] -= 1;//至于这句话为什么不紧挨着写入操作，而是在这里：因为这相当于对缓存槽位提前关门，如果不在读表的临界区内关门的话，读取线程有机会乱入其中，就开始读取了.也可以在①处
        sequence_lock.store(false);//维护次序表的临界区结束

        data.copyTo(buffer[update_idx]);//与克隆速度差不多，正确的使用了内存池，慢的原因可能在于遍历读取与写入
        buffer_status[update_idx] += 1;//修改完了，将这一个位置的写入状态解除，从-1加到0

        for (int i = 0; i < SIZE - 2; i++)
        {
            newdata_flags[i].store(true);//将记录是否有新数据的开关设置为是，表示有新数据到来且准备好了
        }
        return;
    }

    bool ready(cv::Mat& data, int reader = 1) {
        reader--;//为了方便外部表示，读者从1开始编号
        if (!newdata_flags[reader].exchange(false))return false;//考虑到可能会发生的次序混乱，进行的改进
        //因为要读取次序表，临界区开始
        while (sequence_lock.exchange(true));
        //按照次序表从前往后找，先查询最新数据是否可读。
        int newer_data_idx = -1;//较新的数据索引
        for (int i = 0; i < SIZE - 1; i++)
        {
            if (buffer_status[sequence[i]] >= 0)//大于等于零，也可以写成为真
            {
                newer_data_idx = sequence[i];
                break;
            }
        }
        if (newer_data_idx == -1)return false;
        buffer_status[newer_data_idx] += 1;//表示又来了一个读者.也可以在①处
        sequence_lock.store(false);//查询次序表的临界区结束

        buffer[newer_data_idx].copyTo(data);//拷贝到data里才行
        buffer_status[newer_data_idx] -= 1;//读完了，走掉一个读者

        return true;
    }
    ~NoWaitBuffer() {
        delete[] data;
    }
};
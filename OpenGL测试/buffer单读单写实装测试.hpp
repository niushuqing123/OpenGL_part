#include <iostream>
//#include <atomic>
#include <thread>
#include <opencv2/opencv.hpp>

template<int SIZE>
class NoWaitBuffer {
private:
    uchar* data;
    cv::Mat buffer[SIZE];
    std::atomic<int> buffer_status[SIZE];//-1����д/0����/1��һ������/2����������/...
    int sequence[SIZE];
    std::atomic<bool> sequence_lock;//TRUE����������false���ǿ���
    std::atomic<bool> newdata_flags[SIZE - 2];//���������ж��ߵ�һ���־
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
        int update_idx = -1;//��Ҫ��һ�����Ͼɣ��ҿ��õ�λ�ø���������
        int i_s = -1;//��¼update_idx��˳�����ʲôλ���ҵ���
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
        //ͬʱ����˳�����������޸ĵ�λ����Ϊ�������ݵ�λ��,�����update_idx�ƶ�����λ���м������ȫ������һλ��ֱ����ȫԭ���Ŀ�϶��
        for (int i = i_s; i > 0; i--)
        {
            sequence[i] = sequence[i - 1];
        }sequence[0] = update_idx;

        buffer_status[update_idx] -= 1;//������仰Ϊʲô��������д������������������Ϊ���൱�ڶԻ����λ��ǰ���ţ�������ڶ�����ٽ����ڹ��ŵĻ�����ȡ�߳��л����������У��Ϳ�ʼ��ȡ��.Ҳ�����ڢٴ�
        sequence_lock.store(false);//ά���������ٽ�������

        data.copyTo(buffer[update_idx]);//���¡�ٶȲ�࣬��ȷ��ʹ�����ڴ�أ�����ԭ��������ڱ�����ȡ��д��
        buffer_status[update_idx] += 1;//�޸����ˣ�����һ��λ�õ�д��״̬�������-1�ӵ�0

        for (int i = 0; i < SIZE - 2; i++)
        {
            newdata_flags[i].store(true);//����¼�Ƿ��������ݵĿ�������Ϊ�ǣ���ʾ�������ݵ�����׼������
        }
        return;
    }

    bool ready(cv::Mat& data, int reader = 1) {
        reader--;//Ϊ�˷����ⲿ��ʾ�����ߴ�1��ʼ���
        if (!newdata_flags[reader].exchange(false))return false;//���ǵ����ܻᷢ���Ĵ�����ң����еĸĽ�
        //��ΪҪ��ȡ������ٽ�����ʼ
        while (sequence_lock.exchange(true));
        //���մ�����ǰ�����ң��Ȳ�ѯ���������Ƿ�ɶ���
        int newer_data_idx = -1;//���µ���������
        for (int i = 0; i < SIZE - 1; i++)
        {
            if (buffer_status[sequence[i]] >= 0)//���ڵ����㣬Ҳ����д��Ϊ��
            {
                newer_data_idx = sequence[i];
                break;
            }
        }
        if (newer_data_idx == -1)return false;
        buffer_status[newer_data_idx] += 1;//��ʾ������һ������.Ҳ�����ڢٴ�
        sequence_lock.store(false);//��ѯ�������ٽ�������

        buffer[newer_data_idx].copyTo(data);//������data�����
        buffer_status[newer_data_idx] -= 1;//�����ˣ��ߵ�һ������

        return true;
    }
    ~NoWaitBuffer() {
        delete[] data;
    }
};
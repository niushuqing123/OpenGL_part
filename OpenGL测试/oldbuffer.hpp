#pragma once
#include <opencv2/opencv.hpp>

#include<iostream>

#include <chrono>
#include <thread>

#include "time.hpp"

//include缓冲区数据结构
//头文件多了之后合并成一个.hpp放到头文件里
using namespace std;


class Buffer
{//单一变量的缓冲区相当于是一存一拿模式，当存取冲突时，最坏情况是每一步都存在一次数据拷贝的时间开销
public:
    MYtime ttold;

    int val;
    std::atomic<int> new_data;
    cv::Mat img;

    std::atomic<int> reading;
    std::atomic<int> writing;
    int test_num = 0;

    Buffer()
    {
        this->test_num = 0;
        this->reading = false;
        this->writing = false;
        this->new_data = false;
        val = 0;

    }

    void produce(cv::Mat& new_img)
    {
        while (reading.load())test_num++;//因为图片赋值操作耗时非常小，所以几乎不会卡住，但为了严谨还是留着
        //更新了新数据之后
        writing = true;

        ttold.locality_Start();
        this->img = new_img;
        double t = ttold.locality_End();
        //cout << "t:" << t * 1000 << endl;

        writing = false;

        this->new_data = true;
    }

    void consume(cv::Mat& img_for_texture)
    {
        //可能的耗时就在于图片赋值操作上
        //MYtime t;
        //t.locality_start();

        while (writing.load())test_num++;//因为图片赋值操作耗时非常小，所以几乎不会卡住，但为了严谨还是留着

        reading = true;
        img_for_texture = this->img;
        //Sleep(10);//这里如果有延迟，那么上面的自旋锁就能卡住了。

        reading = false;
        //double tt = t.locality_End() * 1000;
        //cout << "tt" << tt << endl;
        //经测试，耗时非常少，0.001毫秒级别

        this->new_data = false;

    }

    bool Buffer_ready()
    {
        return new_data;
    }

};
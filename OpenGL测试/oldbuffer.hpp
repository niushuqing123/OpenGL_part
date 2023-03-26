#pragma once
#include <opencv2/opencv.hpp>

#include<iostream>

#include <chrono>
#include <thread>

#include "time.hpp"

//include���������ݽṹ
//ͷ�ļ�����֮��ϲ���һ��.hpp�ŵ�ͷ�ļ���
using namespace std;


class Buffer
{//��һ�����Ļ������൱����һ��һ��ģʽ������ȡ��ͻʱ��������ÿһ��������һ�����ݿ�����ʱ�俪��
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
        while (reading.load())test_num++;//��ΪͼƬ��ֵ������ʱ�ǳ�С�����Լ������Ῠס����Ϊ���Ͻ���������
        //������������֮��
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
        //���ܵĺ�ʱ������ͼƬ��ֵ������
        //MYtime t;
        //t.locality_start();

        while (writing.load())test_num++;//��ΪͼƬ��ֵ������ʱ�ǳ�С�����Լ������Ῠס����Ϊ���Ͻ���������

        reading = true;
        img_for_texture = this->img;
        //Sleep(10);//����������ӳ٣���ô��������������ܿ�ס�ˡ�

        reading = false;
        //double tt = t.locality_End() * 1000;
        //cout << "tt" << tt << endl;
        //�����ԣ���ʱ�ǳ��٣�0.001���뼶��

        this->new_data = false;

    }

    bool Buffer_ready()
    {
        return new_data;
    }

};
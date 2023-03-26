#pragma once
#include <windows.h> 
#include <iostream> 
#include <cassert>

using namespace std;


class MYtime {
private:
	LARGE_INTEGER locality_cpuFreq;
	LARGE_INTEGER locality_startTime;
	LARGE_INTEGER locality_endTime;
	double locality_ResultTime;
	bool sequence = true;//�����˿�ʼ�ͽ�����˳�򣬲���������ʼ������������
public:
	//��ʼ��ʱ��
	inline void locality_Start()
	{
		if (sequence != true)
		{
			cout << "=================��Ҫ��end֮ǰ����ʹ��start!================" << endl;
			assert(false);//�˳�����
		}
		QueryPerformanceFrequency(&this->locality_cpuFreq);
		QueryPerformanceCounter(&this->locality_startTime);
		sequence = false;
	}
	//������ʱ�����ؽ����������ӡ���
	inline double locality_End()
	{
		if (sequence != false)
		{
			cout << "=================end֮ǰ������ƥ���start!================" << endl;
			assert(false);//�˳�����
		}
		QueryPerformanceCounter(&this->locality_endTime);
		this->locality_ResultTime = (double)(locality_endTime.QuadPart - this->locality_startTime.QuadPart) / this->locality_cpuFreq.QuadPart;
		sequence = true;
		return locality_Result();
	}

	//locality_End�Ѱ����˺������ܣ������ٴ�ȡ���ϴμ�ʱ�����ʹ�ô˺���
	inline double locality_Result()
	{
		//printf("�ֲ������ʱ(s):%0.8f\n", this->locality_ResultTime);
		//�д��󣡣�������������ֵ�ĵ�λ���룬����ms
		//cout << "�ֲ������ʱ��" << this->locality_ResultTime << endl;
		return this->locality_ResultTime;
	}
};

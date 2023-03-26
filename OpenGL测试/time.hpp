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
	bool sequence = true;//限制了开始和结束的顺序，不能连续开始或者连续结束
public:
	//开始计时：
	inline void locality_Start()
	{
		if (sequence != true)
		{
			cout << "=================不要在end之前连续使用start!================" << endl;
			assert(false);//退出程序
		}
		QueryPerformanceFrequency(&this->locality_cpuFreq);
		QueryPerformanceCounter(&this->locality_startTime);
		sequence = false;
	}
	//结束计时并返回结果，包含打印结果
	inline double locality_End()
	{
		if (sequence != false)
		{
			cout << "=================end之前必须有匹配的start!================" << endl;
			assert(false);//退出程序
		}
		QueryPerformanceCounter(&this->locality_endTime);
		this->locality_ResultTime = (double)(locality_endTime.QuadPart - this->locality_startTime.QuadPart) / this->locality_cpuFreq.QuadPart;
		sequence = true;
		return locality_Result();
	}

	//locality_End已包含此函数功能，如需再次取得上次计时结果则使用此函数
	inline double locality_Result()
	{
		//printf("局部区域耗时(s):%0.8f\n", this->locality_ResultTime);
		//有错误！！！！！！返回值的单位是秒，不是ms
		//cout << "局部区域耗时：" << this->locality_ResultTime << endl;
		return this->locality_ResultTime;
	}
};

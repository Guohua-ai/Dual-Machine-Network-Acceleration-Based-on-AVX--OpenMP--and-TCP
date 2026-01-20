#include <iostream>
#include <math.h>
#include <windows.h>
#include "general.h"
#include "fun.h"
#include "funSpeedUp.h"
using namespace std;

int main()
{
	/* 数据初始化 */
	float* rawFloatData = new float[DATANUM];

	for (size_t i = 0; i < DATANUM; i++)
		rawFloatData[i] = float(i + 1);
	
	cout << "Data initialization done!" << endl;
	cout << endl;

	/* 三个功能分别进行单机加速测试 */
	LARGE_INTEGER start, end, freq;

	cout << "----------- SUM SPEEDUP TEST ------------" << endl;

	QueryPerformanceCounter(&start);//start
	QueryPerformanceFrequency(&freq);
	float sumResult = sum(rawFloatData, 0, DATANUM);
	QueryPerformanceCounter(&end);//end
	double sumTime = (double)(end.QuadPart - start.QuadPart) / (double)(freq.QuadPart);
	cout << "Sum Result: " << sumResult << endl;
	cout << "Sum Time Consumed:  " << sumTime << endl;

	QueryPerformanceCounter(&start);//start
	QueryPerformanceFrequency(&freq);
	float sumSpeedUpResult = sumSpeedUp(rawFloatData, DATANUM);
	QueryPerformanceCounter(&end);//end
	double sumSpeedUpTime = (double)(end.QuadPart - start.QuadPart) / (double)(freq.QuadPart);
	cout << "Sum Speedup Result: " << sumSpeedUpResult << endl;
	cout << "Sum Speedup Time Consumed: " << sumSpeedUpTime << endl;
	cout << endl;

	cout << "Sum Speedup Ratio: " << sumTime / sumSpeedUpTime << endl;
	cout << endl;


	cout << "----------- MAX SPEEDUP TEST ------------" << endl;

	QueryPerformanceCounter(&start);//start
	QueryPerformanceFrequency(&freq);
	float maxResult = findMax(rawFloatData, DATANUM);
	QueryPerformanceCounter(&end);//end
	double maxTime = (double)(end.QuadPart - start.QuadPart) / (double)(freq.QuadPart);
	cout << "Max Result: " << maxResult << endl;
	cout << "Max Time Consumed: " << maxTime << endl;

	QueryPerformanceCounter(&start);//start
	QueryPerformanceFrequency(&freq);
	float maxSpeedUpResult = findMaxSpeedUp(rawFloatData, DATANUM);
	QueryPerformanceCounter(&end);//end
	double maxSpeedUpTime = (double)(end.QuadPart - start.QuadPart) / (double)(freq.QuadPart);
	cout << "Max Speedup Result: " << maxSpeedUpResult << endl;
	cout << "Max Speedup Time Consumed: " << maxSpeedUpTime << endl;
	cout << endl;

	cout << "Max Speedup Ratio: " << maxTime / maxSpeedUpTime << endl;
	cout << endl;


	cout << "----------- SORT SPEEDUP TEST -----------" << endl;

	QueryPerformanceCounter(&start);//start
	QueryPerformanceFrequency(&freq);
	float* sortResult = new float[DATANUM];
	sort(rawFloatData, DATANUM, sortResult);
	QueryPerformanceCounter(&end);//end
	bool sortResultCheck = sortCheck(sortResult, DATANUM);
	double sortTime = (double)(end.QuadPart - start.QuadPart) / (double)(freq.QuadPart);
	cout << "Sort Result: " << sortResultCheck << endl;
	cout << "Sort Time Consumed: " << sortTime << endl;

	QueryPerformanceCounter(&start);//start
	QueryPerformanceFrequency(&freq);
	
	
	float* sortSpeedUpResult = new float[DATANUM];
	sortSpeedUp(rawFloatData, DATANUM, sortSpeedUpResult);
	QueryPerformanceCounter(&end);//end
	bool sortSpeedUpResultCheck = sortCheck(sortSpeedUpResult, DATANUM);
	double sortSpeedUpTime = (double)(end.QuadPart - start.QuadPart) / (double)(freq.QuadPart);
	cout << "Sort Speedup Result: " << sortSpeedUpResultCheck << endl;
	cout << "Sort Speedup Time Consumed: " << sortSpeedUpTime << endl;
	cout << endl;

	cout << "Sort Speedup Ratio: " << sortTime / sortSpeedUpTime << endl;
	cout << endl;


	delete[] rawFloatData;
	delete[] sortResult;
	delete[] sortSpeedUpResult;
	return 0;
}
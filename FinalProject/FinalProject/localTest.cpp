#include <iostream>
#include <math.h>
#include <windows.h>
#include "general.h"
#include "fun.h"
using namespace std;

int main()
{
	/* 数据初始化 */
	float* rawFloatData = new float[DATANUM];

	for (size_t i = 0; i < DATANUM; i++)
		rawFloatData[i] = float(i + 1);

	cout << "Data initialization done!" << endl;
	cout << endl;

	/* 单机未加速测试 */
	cout << "------------ LOCAL FUN TEST -------------" << endl;

	LARGE_INTEGER start, end, freq;

	QueryPerformanceCounter(&start);//start
	QueryPerformanceFrequency(&freq);
	
	float sumResult = sum(rawFloatData, 0, DATANUM);
	cout << "Sum Result: " << sumResult << endl;

	float maxResult = findMax(rawFloatData, DATANUM);
	cout << "Max Result: " << maxResult << endl;
	
	float* sortResult = new float[DATANUM];
	sort(rawFloatData, DATANUM, sortResult);
	bool sortResultCheck = sortCheck(sortResult, DATANUM);
	cout << "Sort Result: " << sortResultCheck << endl;
	
	QueryPerformanceCounter(&end);//end
	
	double totalTime = (double)(end.QuadPart - start.QuadPart) / (double)(freq.QuadPart);
	cout << "Time Consumed: " << totalTime << endl;

	delete[] rawFloatData;
	delete[] sortResult;
	return 0;
}
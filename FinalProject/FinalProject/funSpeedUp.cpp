#include <iostream>
#include <math.h>
#include <intrin.h>  
#include <immintrin.h>
#include "general.h"

void logSqrtAVX(const float data[], const int len, float result[]) 
{
	int i = 0;

	// 每次处理 8 个 float (256-bit)
	for (; i + 7 < len; i += 8) {
		__m256 input = _mm256_loadu_ps(&data[i]); // 加载数据
		__m256 sqrt_result = _mm256_sqrt_ps(input); // 计算 sqrt
		__m256 log_result = _mm256_log_ps(sqrt_result); // 计算 log(sqrt(x))
		_mm256_storeu_ps(&result[i], log_result); // 存储结果
	}

	// 处理剩余的数据
	for (; i < len; i++) {
		result[i] = log(sqrt(data[i])); // 如果数据长度不是 8 的倍数，使用标量计算
	}
}

float sumAVX(const float data[], const int len)
{
	float result = 0;
	const int blockLen = 8;
	const int blockNum = len / blockLen;

	__m256 block = _mm256_setzero_ps();
	const float* p = data;

	for (int i = 0; i < blockNum; i++)
	{
		block = _mm256_add_ps(block, _mm256_log_ps(_mm256_sqrt_ps(_mm256_loadu_ps(p))));
		p += blockLen;
	}

	float* blockResult = (float*)&block;
	for (int i = 0; i < blockLen; i++)
		result += blockResult[i];

	return result;
}

float sumSpeedUp(const float data[], const int len)
{
	float result = 0; 
	float threadResult[MAX_THREADS]{ 0 };
	const int threadLen = len / MAX_THREADS;

#pragma omp parallel for 
	for (int i = 0; i < MAX_THREADS; i++) //为提升精度，对每个线程的数据再分两块调用sumAVX
		threadResult[i] = sumAVX(data + threadLen * i, threadLen / 2) + sumAVX(data + threadLen * i + threadLen / 2, threadLen / 2);

	for (int i = 0; i < MAX_THREADS; i++)
		result += threadResult[i];

	return result; 
}

float maxAVX(const float data[], const int len)
{
	float result = log(sqrt(data[0]));
	const int blockLen = 8;
	const int blockNum = len / blockLen;

	__m256 block = _mm256_setzero_ps();
	const float* p = data;
	block = _mm256_log_ps(_mm256_sqrt_ps(_mm256_loadu_ps(p)));

	for (int i = 1; i < blockNum; i++)
	{
		block = _mm256_max_ps(block, _mm256_log_ps(_mm256_sqrt_ps(_mm256_loadu_ps(p))));
		p += blockLen;
	}

	float* blockResult = (float*)&block;
	for (int i = 0; i < blockLen; i++)
		if (blockResult[i] > result)
			result = blockResult[i];

	return result;
}

float findMaxSpeedUp(const float data[], const int len)
{
	float result = log(sqrt(data[0]));
	float threadResult[MAX_THREADS]{ 0 };
	const int threadLen = len / MAX_THREADS;

#pragma omp parallel for 
	for (int i = 0; i < MAX_THREADS; i++)
		threadResult[i] = maxAVX(data + threadLen * i, threadLen);

	for (int i = 0; i < MAX_THREADS; i++)
		if (threadResult[i] > result)
			result = threadResult[i];

	return result;
}

void sortSpeedUp(const float data[], int len, float result[])
{
	logSqrtAVX(data, len, result);
	float t, * temp;
	temp = (float*)malloc(len * sizeof(float));
	if (temp == nullptr) {
		std::cerr << "内存分配失败！" << std::endl;
		exit(1);  // 如果分配失败，退出程序
	}

	//这里做了一些优化，预处理合并了单个的区间，略微提高的速度
#pragma omp parallel for private(t)
	for (int i = 0; i < len / 2; i++) {
		if (result[i * 2] > result[i * 2 + 1]) {
			t = result[i * 2];
			result[i * 2] = result[i * 2 + 1];
			result[i * 2 + 1] = t;
		}
	}

	//i代表每次归并的区间长度，j代表需要归并的两个区间中最小的下标
	for (int i = 2; i < len; i *= 2) {
#pragma omp parallel for
		for (int j = 0; j < len - i; j += i * 2) {
			merge(result, j, j + i, (j + i * 2 < len ? j + i * 2 : len), temp);
		}
	}

	free(temp);
}

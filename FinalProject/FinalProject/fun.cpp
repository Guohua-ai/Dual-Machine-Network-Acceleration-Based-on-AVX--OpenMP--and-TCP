#include <iostream>
#include <math.h>
#include "general.h"

void logSqrt(const float data[], const int len, float result[])
{
	for (int i = 0; i < len; i++)
		result[i] = log(sqrt(data[i]));
}

float sum(const float data[], int start, int end)
{
	float result = 0;
	const int blockLen = 64;

	if (end - start <= blockLen) {
		for (int i = start; i < end; i++)
			result += log(sqrt(data[i]));
		return result;
	}

	int mid = (start + end) / 2;
	float leftResult = sum(data, start, mid);
	float rightResult = sum(data, mid, end);
	result = leftResult + rightResult;
	return result;
}

float findMax(const float data[], const int len)
{
	float result = log(sqrt(data[0]));
	for (int i = 1; i < len; i++)
		if (log(sqrt(data[i])) > result)
			result = log(sqrt(data[i]));
	return result;
}

// 归并排序的递归函数
void mergeSort(float* data, int left, int right, float* temp) {
	if (left < right - 1) {  // 确保至少有两个元素可以排序
		int mid = left + (right - left) / 2;

		// 递归分割
		mergeSort(data, left, mid, temp);
		mergeSort(data, mid, right, temp);

		// 合并
		merge(data, left, mid, right, temp);
	}
}

// 对原数据进行排序处理
void sort(const float data[], int len, float result[]) {
	// 对每个元素执行 log(sqrt(x)) 操作
	for (int i = 0; i < len; i++) {
		result[i] = log(sqrt(data[i]));
	}

	// 分配一次性临时数组
	float* temp = (float*)malloc(len * sizeof(float));
	if (temp == nullptr) {
		std::cerr << "内存分配失败！" << std::endl;
		exit(1);
	}

	// 排序 result 数组
	mergeSort(result, 0, len, temp);

	// 释放临时数组
	free(temp);
}

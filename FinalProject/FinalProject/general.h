#pragma once

#define MAX_THREADS 64
#define SUBDATANUM 2000000
#define DATANUM (SUBDATANUM * MAX_THREADS)

bool sortCheck(const float data[], const int len);
void merge(float data[], int l1, int r1, int r2, float* temp);
void merge2(float data1[], float data2[], float result[], int len1, int len2, float* temp);
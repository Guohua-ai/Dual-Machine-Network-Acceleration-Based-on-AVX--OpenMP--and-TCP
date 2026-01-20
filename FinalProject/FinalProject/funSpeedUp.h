#pragma once

void logSqrtAVX(const float data[], const int len, float result[]);
float sumSpeedUp(const float data[], const int len); //data是原始数据，len为长度。结果通过函数返回
float findMaxSpeedUp(const float data[], const int len);//data是原始数据，len为长度。结果通过函数返回
void sortSpeedUp(const float data[], int len, float result[]);
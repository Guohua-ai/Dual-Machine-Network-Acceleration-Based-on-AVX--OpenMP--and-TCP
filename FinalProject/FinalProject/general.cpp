bool sortCheck(const float data[], const int len)
{
	bool isSorted = true;

	for (int i = 1; i < len; i++)
		if (data[i - 1] > data[i]) {
			isSorted = false;
			break;
		}

	return isSorted;
}

void merge(float data[], int l1, int r1, int r2, float* temp)
{
	int top = l1, p = l1, q = r1;
	while (p < r1 || q < r2) {
		if (q >= r2 || (p < r1 && data[p] <= data[q])) {
			temp[top++] = data[p++];
		}
		else {
			temp[top++] = data[q++];
		}
	}
	for (top = l1; top < r2; top++) {
		data[top] = temp[top];
	}
}

void merge2(float data1[], float data2[], float result[], int len1, int len2, float* temp) {
    int p = 0, q = 0, top = 0;

    while (p < len1 && q < len2) {
        if (data1[p] <= data2[q]) {
            result[top++] = data1[p++];
        }
        else {
            result[top++] = data2[q++];
        }
    }

    while (p < len1) {
        result[top++] = data1[p++];
    }

    while (q < len2) {
        result[top++] = data2[q++];
    }
}
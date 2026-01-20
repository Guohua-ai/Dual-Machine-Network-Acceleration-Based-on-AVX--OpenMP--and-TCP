//TCP client
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "general.h"
#include "fun.h"
#include "funSpeedUp.h"
using namespace std;

#define SERVER_IP "172.20.10.2"	//请修改为服务器ip地址
#define SERVER_PORT 8888

int main()
{
	/* 数据初始化 */
	float* rawFloatData = new float[DATANUM / 2];

	for (size_t i = 0; i < DATANUM / 2; i++)
		rawFloatData[i] = float(i + DATANUM / 2 + 1);

	cout << "Data initialization done!" << endl;

	/* TCP连接 */
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBoxA(NULL, "Winsock startup error", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	SOCKADDR_IN serverAddr; //Adres przypisany do socketu Connection
	int sizeofaddr = sizeof(serverAddr);
	serverAddr.sin_family = AF_INET; //IPv4 Socket
	serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP); //服务器的ip地址
	serverAddr.sin_port = htons(SERVER_PORT); //服务器的Port

	SOCKET Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&serverAddr, sizeofaddr) != 0) //连接服务器，三次握手
	{
		MessageBoxA(NULL, "Bad Connection", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	/* 等待开始计算指令 */
	char flag = 0;
	while (1)
	{
		recv(Connection, &flag, sizeof(char), NULL);
		if (flag == 1)
		{
			cout << "Signal received!" << endl;
			break;
		}
	}

	/* 开始本机计算 */
	cout << "----------- LOCAL COMPUTING START ------------" << endl;
	float cltSumResult = sumSpeedUp(rawFloatData, DATANUM / 2);
	cout << "Client Sum: " << cltSumResult << endl;
	float cltMaxResult = findMaxSpeedUp(rawFloatData, DATANUM / 2);
	cout << "Client Max: " << cltMaxResult << endl;
	float* cltSortResult = new float[DATANUM / 2];
	sortSpeedUp(rawFloatData, DATANUM / 2, cltSortResult);
	bool cltSortResultCheck = sortCheck(cltSortResult, DATANUM / 2);
	cout << "Client Sort: " << cltSortResultCheck << endl;
	cout << endl;

	/* 准备发送数据 */
	cout << "-------------- DATA SEND START ---------------" << endl;
	const int dataSize = (DATANUM / 2 + 2) * sizeof(float);
	cout << "Data Size: " << dataSize << endl;

	float* resultBuf = new float[dataSize] {0};
	resultBuf[0] = cltSumResult;
	resultBuf[1] = cltMaxResult;
	for (int i = 0; i < DATANUM / 2; i++)
		resultBuf[i + 2] = cltSortResult[i];
	char* sendBuf = new char[dataSize] {0};
	memcpy(sendBuf, resultBuf, dataSize);

	/* 开始向server发送数据 */
	const int packageSize = 4 * 1024 * 1024; // 每个包的大小为4MB
	char* p = sendBuf;
	int sentSize = 0;
	int remainSize = dataSize;
	int packageNum = 0;

	while (remainSize > 0)
	{
		// 计算每次发送的数据大小
		int size = (remainSize > packageSize) ? packageSize : remainSize;

		int sent = send(Connection, p, size, NULL);
		if (sent == SOCKET_ERROR) {
			cout << "Send failed!" << endl;
			break;
		}

		remainSize -= sent;
		p += sent;
		packageNum++;
		sentSize += sent;
	}
	cout << "Sent size:" << sentSize << endl;

	delete[] rawFloatData;
	delete[] cltSortResult;
	delete[] sendBuf;
	delete[] resultBuf;

	//cleanup
	closesocket(Connection);
	WSACleanup();
	return 0;
}

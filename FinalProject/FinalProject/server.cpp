//TCP server
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <iostream>
#include <string>
#include <iostream>
#include "general.h"
#include "fun.h"
#include "funSpeedUp.h"
using namespace std;

#define SERVER_IP "172.20.10.3"	//请修改为服务器ip地址
#define SERVER_PORT 8888

int main()
{
	/* 数据初始化 */
	float* rawFloatData = new float[DATANUM / 2];

	for (size_t i = 0; i < DATANUM / 2; i++)
		rawFloatData[i] = float(i + 1);

	cout << "Data initialization done!" << endl;

	/* TCP连接 */
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBoxA(NULL, "WinSock startup error", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	SOCKADDR_IN addr;
	int addrlen = sizeof(addr);
	addr.sin_family = AF_INET; //IPv4 Socket
	addr.sin_port = htons(SERVER_PORT); // server Port
	addr.sin_addr.s_addr = inet_addr(SERVER_IP); //server PC

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);//公司的接线员，不是处理业务的人 
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr)); //服务器必须绑定

	listen(sListen, SOMAXCONN); //接线员聆听是否有生意

	SOCKET newConnection; //真正的业务员  build a new socket do new connection. the sListen is just listenning not used to exchange data
	newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen); //newConnection is used to exchange data with client
	if (newConnection == 0)
	{
		std::cout << "bad connection." << std::endl;
		return 0;
	}
		
	LARGE_INTEGER start, end, freq;
	/* 开始计时 */
	QueryPerformanceCounter(&start);//start
	QueryPerformanceFrequency(&freq);

	/* 发送开始计算指令 */
	const char sendData = 1;
	send(newConnection, &sendData, sizeof(char), NULL);
	cout << "Signal sent!" << endl;
	cout << endl;

	/* 开始本机计算 */
	cout << "----------- LOCAL COMPUTING START ------------" << endl;
	float srvSumResult = sumSpeedUp(rawFloatData, DATANUM / 2);
	cout << "Server Sum: " << srvSumResult << endl;
	float srvMaxResult = findMaxSpeedUp(rawFloatData, DATANUM / 2);
	cout << "Server Max: " << srvMaxResult << endl;
	float* srvSortResult = new float[DATANUM / 2] {0};
	sortSpeedUp(rawFloatData, DATANUM / 2, srvSortResult);
	bool srvSortResultCheck = sortCheck(srvSortResult, DATANUM / 2);
	cout << "Server Sort: " << srvSortResultCheck << endl;
	cout << endl;

	/* 开始接收client数据 */
	cout << "-------------- DATA RECV START ---------------" << endl;
	const int buffer = 64 * 1024 * 1024;
	setsockopt(newConnection, IPPROTO_TCP, TCP_NODELAY, (const char*)&buffer, sizeof(int));

	const int dataSize = (DATANUM / 2 + 2) * sizeof(float);
	cout << "Data Size: " << dataSize << endl;

	char* recvBuf = new char[dataSize] {0};
	char* p = recvBuf;
	int recvSize = 0;
	int remainSize = dataSize;
	int packageNum = 0;

	while (remainSize > 0)
	{
		int size = recv(newConnection, p, remainSize, NULL);
		remainSize -= size;
		p += size;
		packageNum++;
		recvSize += size;
	}
	cout << "Received size: " << recvSize << endl;
	cout << "Package number: " << packageNum << endl;
	cout << endl;
	
	
	/* 提取接收数据 */
	cout << "----------- DATA PROCESSING START ------------" << endl;
	float* resultBuf = new float[DATANUM / 2 + 2] {0};
	memcpy(resultBuf, recvBuf, dataSize);
	float cltSumResult = resultBuf[0];
	float cltMaxResult = resultBuf[1];
	float* cltSortResult = new float[DATANUM / 2] {0};
	for (int i = 0; i < DATANUM / 2; i++)
		cltSortResult[i] = resultBuf[i + 2];

	float sumResult = srvSumResult + cltSumResult;
	cout << "Sum Result: " << sumResult << endl;
	float maxResult = (srvMaxResult > cltMaxResult ? srvMaxResult : cltMaxResult);
	cout << "Max Result: " << maxResult << endl;
	float* sortResult = new float[DATANUM];
	float* temp = new float[DATANUM];
	merge2(srvSortResult, cltSortResult, sortResult, DATANUM / 2, DATANUM / 2, temp);

	bool sortResultCheck = sortCheck(sortResult, DATANUM);
	cout << "Sort Result: " << sortResultCheck << endl;

	QueryPerformanceCounter(&end);//end

	double totalTime = (double)(end.QuadPart - start.QuadPart) / (double)(freq.QuadPart);
	cout << "Time Consumed: " << totalTime << endl;

	delete[] rawFloatData;
	delete[] srvSortResult;
	delete[] cltSortResult;
	delete[] recvBuf;
	delete[] resultBuf;
	delete[] temp;

	//cleanup
	closesocket(newConnection);
	WSACleanup();
	return 0;
}
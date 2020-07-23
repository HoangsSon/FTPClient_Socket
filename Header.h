#pragma once
#pragma comment(lib, "Ws2_32.lib")
#pragma warning (disable : 4996)

#include <string>
#include <iostream>
#include <winsock.h>
#include <string.h>
#include <fstream>

#define _CRT_SECURE_NO_WARNINGS
#define IP "10.81.6.137"
#define PORT 21
#define BUFF_LEN 2048

using namespace std;

void clearBuf(char* s) {
	for (int i = 0; i < BUFF_LEN; i++)
		s[i] = ' ';
}

int endStrIndex(char* s) {
	for (int i = BUFF_LEN - 1; i >= 0; i--)
		if (s[i] == '\n') return i;
}

void _Print(char* buff) {
	
	for (int i = 0; i <= endStrIndex(buff); i++)
		cout << buff[i];
	cout << '\n';
}

void serverResponse(int socketData, char* buff) {
	recv(socketData, buff, strlen(buff), 0); 
	_Print(buff);
}

int _dataPort(int sock, char* buff) {
	if (send(sock, "PASV\r\n", 6, 0) < 0)
	{
		cout << WSAGetLastError;
		closesocket(sock);
		WSACleanup();
		return 1;
	}
	serverResponse(sock, buff);

	///         Get PASSIVE PORT        /// 
	int lennewB = endStrIndex(buff) - 29;
	char* newB = new char[lennewB];
	int j = 0;
	for (int i = 27; i < endStrIndex(buff) - 3; i++) {
		newB[j++] = buff[i];
	}
	newB[j] = '\0';

	char* s = strtok(newB, ",");  // ip1
	s = strtok(NULL, ",");  //ip2
	s = strtok(NULL, ",");  //ip3
	s = strtok(NULL, ",");  //ip4
	s = strtok(NULL, ",");  //p1
	int a = atoi(s);   // p1 - int
	s = strtok(NULL, ",");  // p2
	int b = atoi(s);   // p2 - int

			/// PASSIVE PORT  ///
	return a * 256 + b;
}
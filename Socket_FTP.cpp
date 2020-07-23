#include "Header.h"

int main() {
			///                                 /// 
	WSADATA WinSockData;
	WSAStartup(MAKEWORD(2, 2), &WinSockData);

	
	char messUSER[20] = "USER seed\r\n";
	char messPASS[20] = "PASS dees\r\n";
	char buff[BUFF_LEN];


			///        Control SOCKET          ///
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		cout << WSAGetLastError;
		closesocket(sock);
		WSACleanup();
		return 1;
	}
			///       IP, PORT Server         ///
	struct sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(IP);
	servAddr.sin_port = htons(PORT);

			///     Conncect to Server     ///
	if (connect(sock, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
		cout << WSAGetLastError;
		closesocket(sock);
		WSACleanup();
		return 1;
	}
	serverResponse(sock, buff);
	clearBuf(buff);

///########################################USER########################################///
	if (send(sock, messUSER, int(strlen(messUSER)), 0) < 0) {
		cout << WSAGetLastError;
		closesocket(sock);
		WSACleanup();
		return 1;
	}
	serverResponse(sock, buff);
	clearBuf(buff);
///########################################USER########################################///




///########################################PASS########################################///
	if (send(sock, messPASS, int(strlen(messPASS)), 0) < 0) {
		cout << WSAGetLastError;
		closesocket(sock);
		WSACleanup();
		return 1;
	}
	serverResponse(sock, buff);
	clearBuf(buff);
///########################################PASS########################################///




///########################################LIST########################################///
			///    PASSIVE PORT    ///
	int dataPort = _dataPort(sock, buff);

			///   IP + PASSIVE PORT SERVER   ///
	struct sockaddr_in servAddr2;
	servAddr2.sin_family = AF_INET;
	servAddr2.sin_addr.s_addr = inet_addr(IP);
	servAddr2.sin_port = htons(dataPort);

			///     DATA SOCKET     ///
	int Datasock = socket(AF_INET, SOCK_STREAM, 0);
	if (Datasock < 0) {
		cout << WSAGetLastError;
		closesocket(sock);
		closesocket(Datasock);
		WSACleanup();
		return 1;
	}
			///     Connect to Server      ///
	if (connect(Datasock, (struct sockaddr*)&servAddr2, sizeof(servAddr2)) < 0) {
		cout << WSAGetLastError;
		closesocket(sock);
		closesocket(Datasock);
		WSACleanup();
		return 1;
	}
	clearBuf(buff);

			///     LIST File        ///
	if (send(sock, "LIST\r\n", 6, 0) < 0) {
		cout << WSAGetLastError;
		closesocket(sock);
		closesocket(Datasock);
		WSACleanup();
		return 1;
	}
	serverResponse(Datasock, buff);
	clearBuf(buff);
	closesocket(Datasock);
	serverResponse(sock, buff);
	clearBuf(buff);
///########################################LIST########################################///

	



///########################################UPLOAD########################################///
	dataPort = _dataPort(sock, buff);
	
			///   IP + PASSIVE PORT SERVER   ///
	servAddr2.sin_family = AF_INET;
	servAddr2.sin_addr.s_addr = inet_addr(IP);
	servAddr2.sin_port = htons(dataPort);

			///     DATA SOCKET     ///
	Datasock = socket(AF_INET, SOCK_STREAM, 0);
	if (Datasock < 0) {
		cout << WSAGetLastError;
		closesocket(Datasock);
		WSACleanup();
		return 1;
	}
			///     Connect to Server    ///
	if (connect(Datasock, (struct sockaddr*)&servAddr2, sizeof(servAddr2)) < 0) {
		cout << WSAGetLastError;
		closesocket(Datasock);
		WSACleanup();
		return 1;
	}
	clearBuf(buff);
	
			///     UPLOAD File     ///
	fstream f;
	f.open("C:\\Users\\17520\\Documents\\hello.txt", ios::in);
	
	if (!f) {
		cout << "Fail to open file\n";
		return 1;
	}

	if (send(sock, "STOU Text.txt\r\n", 15, 0) < 0) {
		cout << WSAGetLastError;
		closesocket(sock);
		closesocket(Datasock);
		WSACleanup();
		return 1;
	}
	serverResponse(sock, buff);
	clearBuf(buff);
	string line;
	char* c;
	while (!f.eof()) {
		getline(f, line);
		c = new char[line.length() + 2];
		strcpy(c, line.c_str());
		c[line.length()] = '\n';
		c[line.length() + 1] = '\0';
		send(Datasock, c, strlen(c), 0);
		clearBuf(buff);
	}
	f.close();
	closesocket(Datasock);
	serverResponse(sock, buff);
	clearBuf(buff);
///########################################UPLOAD########################################///





///########################################DELETE########################################///
			///      DELETE File       ///

	if (send(sock, "DELE hello.txt\r\n", 16, 0) < 0) {
		cout << WSAGetLastError;
		closesocket(sock);
		WSACleanup();
		return 1;
	}
	serverResponse(sock, buff);
	clearBuf(buff);
///########################################DELETE########################################///




	
///########################################DOWNLOAD########################################///
				///   PASSIVE PORT    ///
	dataPort = _dataPort(sock, buff);

			///   IP + PASSIVE PORT SERVER   ///
	servAddr2.sin_family = AF_INET;
	servAddr2.sin_addr.s_addr = inet_addr(IP);
	servAddr2.sin_port = htons(dataPort);

			///     DATA SOCKET     ///
	Datasock = socket(AF_INET, SOCK_STREAM, 0);
	if (Datasock < 0) {
		cout << WSAGetLastError;
		closesocket(sock);
		closesocket(Datasock);
		WSACleanup();
		return 1;
	}
			///     Connect to Server    ///
	if (connect(Datasock, (struct sockaddr*)&servAddr2, sizeof(servAddr2)) < 0) {
		cout << WSAGetLastError;
		closesocket(sock);
		closesocket(Datasock);
		WSACleanup();
		return 1;
	}
	clearBuf(buff);

			///   LOCAL FILE    ///
	FILE* fd = fopen("C:\\Users\\17520\\Documents\\get-pip.py", "ab");
	if (!fd) {
		cout << "Fail to open file\n";
		closesocket(sock);
		closesocket(Datasock);
		return 1;
	}

			///    DOWNLOAD FILE    ///
	if (send(sock, "RETR get-pip.py\r\n", 18, 0) < 0) {
		cout << WSAGetLastError;
		closesocket(sock);
		closesocket(Datasock);
		WSACleanup();
		return 1;
	}
	serverResponse(sock, buff);
	clearBuf(buff);

	while (recv(Datasock, buff, BUFF_LEN, 0) > 0) {
		fwrite(buff, sizeof(char), sizeof(buff), fd);
		clearBuf(buff);
	}
	fclose(fd);
	closesocket(Datasock);
	serverResponse(sock, buff);
	clearBuf(buff);
///########################################DOWNLOAD########################################///



	closesocket(sock);
	WSACleanup();
	return 0;
}
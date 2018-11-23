#include "TCP.h"

int LaunchTcpListener(string portNumber) {
	SOCKET listenSocket = INVALID_SOCKET;
	SOCKET clientSocket = INVALID_SOCKET;

	char receiveBuffer[512];
	int receiveBufferLength = 512;

	struct addrinfo *result = NULL;
	struct addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = IPPROTO_TCP;

	while (1) {
		// Resolve the server address and port
		int functionResult = getaddrinfo(NULL, portNumber.c_str(), &hints, &result);
		if (functionResult != 0) {
			printf("\tgetaddrinfo failed with error: %d\n", functionResult);
			WSACleanup();
			return 1;
		}

		// Create a SOCKET for connecting to server
		listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (listenSocket == INVALID_SOCKET) {
			printf("\tTCP socket failed with error: %ld\n", WSAGetLastError());
			freeaddrinfo(result);
			WSACleanup();
			return 1;
		}
		cout << "\tTCP socket created\n";

		// Setup the TCP listening socket
		functionResult = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
		if (functionResult == SOCKET_ERROR) {
			printf("TCP bind failed with error: %d\n", WSAGetLastError());
			freeaddrinfo(result);
			closesocket(listenSocket);
			WSACleanup();
			return 1;
		}
		cout << "\tTCP bind succesful\n";
		freeaddrinfo(result);

		cout << "\tWaiting for TCP connection\n";
		// Wait for the listening socket
		functionResult = listen(listenSocket, SOMAXCONN);
		if (functionResult == SOCKET_ERROR) {
			printf("\tTCP listen failed with error: %d\n", WSAGetLastError());
			closesocket(listenSocket);
			WSACleanup();
			return 1;
		}

		// Accept a client socket
		clientSocket = accept(listenSocket, NULL, NULL);
		if (clientSocket == INVALID_SOCKET) {
			printf("\tTCP accept failed with error: %d\n", WSAGetLastError());
			closesocket(listenSocket);
			WSACleanup();
			return 1;
		}
		cout << "\tTCP Client connected\n";

		clock_t startTime = clock();
		int elapsedTime = 0;
		fd_set setReadsSelect = { 0 };
		timeval timeout = { 0 };
		timeout.tv_sec = 5;
		while (1) {
			cout << "\tWaiting for TCP command\n";
			FD_SET(clientSocket, &setReadsSelect);
			functionResult = select(0, &setReadsSelect, NULL, NULL, &timeout);
			if (functionResult < 1) {
				// Socket is closed after 5 seconds of inactivity
				cout << "\tClosing TCP socket (5 sec inactivity)\n";
				closesocket(listenSocket);
				break;
			}

			functionResult = recv(clientSocket, receiveBuffer, receiveBufferLength, MSG_PARTIAL);
			cout << "\tTCP command received\n";
			if (functionResult > 0) {
				string serverReply = GetReply(receiveBuffer, functionResult);

				int sendResult = send(clientSocket, serverReply.c_str(), serverReply.length(), 0);
				if (sendResult == SOCKET_ERROR) {
					printf("\tTCP send failed with error: %d\n", WSAGetLastError());
					closesocket(clientSocket);
					WSACleanup();
					return 1;
				}
				cout << "\tTCP Reply sent\n";
			}
			else if (functionResult == 0) {
				if (elapsedTime != ((clock() - startTime) / CLOCKS_PER_SEC)) {
					elapsedTime = (clock() - startTime) / CLOCKS_PER_SEC;
				}
				if (elapsedTime >= 5) {
					// Socket is closed after 5 seconds of inactivity
					cout << "\tClosing TCP socket (5 sec inactivity)\n";
					closesocket(listenSocket);
					break;
				}
			}
			else if (functionResult != 0) {
				printf("\tTCP recv failed with error: %d\n", WSAGetLastError());
				closesocket(clientSocket);
				WSACleanup();
				return 1;
			}
		}
	}
}
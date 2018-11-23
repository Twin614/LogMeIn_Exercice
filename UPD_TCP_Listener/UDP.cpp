#include "UDP.h"

int LaunchUdpListener(string portNumber) {
	SOCKET listenSocket = INVALID_SOCKET;
	sockaddr_in myAddress;
	char receiveBuffer[512];

	// Create a SOCKET for connecting to server
	listenSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (listenSocket == INVALID_SOCKET) {
		printf("UDP socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	cout << "UDP socket created\n";

	myAddress.sin_family = AF_INET;
	myAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	myAddress.sin_port = htons(stoi(portNumber));

	// Setup the UDP listening socket
	int functionResult = bind(listenSocket, (SOCKADDR*)&myAddress, sizeof(myAddress));
	if (functionResult == SOCKET_ERROR) {
		printf("UDP bind failed with error: %d\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}
	cout << "UDP bind succesful\n";

	while (1)
	{
		int serverLength = sizeof(struct sockaddr_in);
		cout << "Waiting for UDP command\n";
		functionResult = recvfrom(listenSocket, receiveBuffer, 512, 0, (SOCKADDR*)&myAddress, &serverLength);
		cout << "UDP Command received\n";
		string serverReply = GetReply(receiveBuffer, functionResult);

		sendto(listenSocket, serverReply.c_str(), serverReply.length(), 0, (SOCKADDR*)&myAddress, serverLength);
		cout << "UDP Reply sent\n";
	}
}
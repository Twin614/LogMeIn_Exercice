#include "TCP.h"
#include "UDP.h"
#include <thread>


int main()
{
	WSADATA wsaData;
	int functionResult;

	SOCKET listenSocket = INVALID_SOCKET;
	SOCKET clientSocket = INVALID_SOCKET;

	int sendResult;
	char receiveBuffer[512];
	int receiveBufferLength = 512;

	// Initialize Winsock
	functionResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (functionResult != 0) {
		printf("WSAStartup failed with error: %d\n", functionResult);
		return 1;
	}	

	string portNumber;
	int port = 0;
	string protocol = "";
	while (port <= 0) {
		cout << "Which port number to listen? ";
		getline(cin, portNumber); // takes input from the user and places it into the string

		try {
			port = stoi(portNumber);
			if (port <= 0) {
				cout << "Invalid port. Port must be positive. Please try again." << endl;
			}
		}
		catch (std::invalid_argument& e) {
			// No conversion could be performed, not a valid port
			cout << "Invalid port. Port must be a number. Please try again." << endl;
		}
		catch (...){
			cout << "Invalid port. Please try again." << endl;
		}
	}

	// you can output the response they gave, like so
	cout << "Listening port: " << portNumber << endl;

	thread upd = thread(LaunchUdpListener, portNumber);
	thread tcp = thread(LaunchTcpListener, portNumber);
	upd.join();
	tcp.join();
	
	// shutdown the connection since we're done
	functionResult = shutdown(clientSocket, SD_SEND);
	if (functionResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(clientSocket);
		WSACleanup();
		return 1;
	}

	// cleanup
	closesocket(clientSocket);
	WSACleanup();

	return 0;
}
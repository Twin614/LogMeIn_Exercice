#pragma once
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <time.h>
#include <cstdlib>
#include <fstream>

using namespace std;

#pragma comment (lib, "Ws2_32.lib")

string GetReply(char receiveBuffer[512], int charCount);

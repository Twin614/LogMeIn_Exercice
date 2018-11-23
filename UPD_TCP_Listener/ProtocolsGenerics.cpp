#include "ProtocolsGenerics.h"

string GetReply(char receiveBuffer[512], int charCount) {
	string receivedCommand(receiveBuffer, charCount);

	auto time = std::time(nullptr);
	struct tm currentTime;
	localtime_s(&currentTime, &time);

	std::ostringstream oss;
	string serverReplies;
	if (receivedCommand.compare("date") == 0) {
		oss << std::put_time(&currentTime, "%F");
		serverReplies = oss.str();
	}
	else if (receivedCommand.compare("time") == 0) {
		oss << std::put_time(&currentTime, "%T");
		oss << std::put_time(&currentTime, "%z");
		serverReplies = oss.str();
	}
	else if (receivedCommand.compare("datetime") == 0) {
		oss << std::put_time(&currentTime, "%F");
		oss << "T" << std::put_time(&currentTime, "%T");
		oss << std::put_time(&currentTime, "%z");
		serverReplies = oss.str();
	}
	else {
		serverReplies = "Wrong entry. Please try again.";
	}
	return serverReplies + "\n";
}
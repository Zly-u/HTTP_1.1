#include "server.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include <cstdio>

#include "Shared/helpers.h"

#pragma comment(lib, "Ws2_32.lib")

namespace server {
	int StartServer(){
		debug::printf("Testing socket\n");

		int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

		sockaddr_in serverAddress;
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_port = htons(8080);
		serverAddress.sin_addr.s_addr = INADDR_ANY;

		if(bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress))) {
			perror("bind");
		}

		return 1;
	}
}
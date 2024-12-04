#include "server.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include <cstdio>

#include "Shared/helpers.h"

#pragma comment(lib, "Ws2_32.lib")

namespace server {
	WSADATA wsaData;
	SOCKET ListenSocket = INVALID_SOCKET;
	int StartServer(){
		debug::printf("Testing socket\n");

		int iResult = 0;
		iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
		if (iResult) {
			debug::printf("WSAStartup failed: %d\n", iResult);
			goto WSAStartup_error;
		}

		addrinfo* result = nullptr;
		addrinfo* ptr = nullptr;
		addrinfo hints;

		ZeroMemory(&hints, sizeof (hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		// Resolve the local address and port to be used by the server
		iResult = getaddrinfo(nullptr, "8080", &hints, &result);
		if (iResult) {
			debug::printf("getaddrinfo failed: %d\n", iResult);
			goto resolve_error;
		}

		ListenSocket = socket(
			result->ai_family,
			result->ai_socktype,
			result->ai_protocol
		);

		if (ListenSocket == INVALID_SOCKET) {
			debug::printf("Error at socket(): %ld\n", WSAGetLastError());
			goto socket_error;
		}

		// Setup the TCP listening socket
		iResult = bind(ListenSocket, result->ai_addr, static_cast<int>(result->ai_addrlen));
		if (iResult == SOCKET_ERROR) {
			debug::printf("bind failed with error: %d\n", WSAGetLastError());
			goto listening_error;
		}

		return 0;

		{
			listening_error:{
				closesocket(ListenSocket);
			}

			socket_error:{
				freeaddrinfo(result);
			}

			resolve_error:{
				WSACleanup();
			}

			WSAStartup_error:

			return 1;
		}
	}
}
#include "server.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include <cstdio>
#include <thread>

#include "Shared/helpers.h"

#pragma comment(lib, "Ws2_32.lib")

namespace server {
	std::jthread m_listening_thread;
	std::jthread m_receiving_thread;

	WSADATA wsaData;
	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;
	int StartServer(){
		debug::printf("Starting Server...\n");


		addrinfo* result = nullptr;
		addrinfo hints;
		int iResult = 0;

		debug::printf("\tWSAStartup...\n");
		iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
		if (iResult) {
			debug::printf("\t\tWSAStartup failed: %d\n", iResult);
			goto WSAStartup_error;
		}

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		debug::printf("\tgetaddrinfo...\n");
		// Resolve the local address and port to be used by the server
		iResult = getaddrinfo(nullptr, "8080", &hints, &result);
		if (iResult) {
			debug::printf("\t\tgetaddrinfo failed: %d\n", iResult);
			goto resolve_error;
		}

		debug::printf("\tsocket()...\n");
		ListenSocket = socket(
			result->ai_family,
			result->ai_socktype,
			result->ai_protocol
		);

		if (ListenSocket == INVALID_SOCKET) {
			debug::printf("\t\tError at socket(): %ld\n", WSAGetLastError());
			goto socket_error;
		}

		// Setup the TCP listening socket
		debug::printf("\tbind()...\n");
		iResult = bind(ListenSocket, result->ai_addr, static_cast<int>(result->ai_addrlen));
		if (iResult == SOCKET_ERROR) {
			debug::printf("\t\tbind failed with error: %d\n", WSAGetLastError());
			goto listening_error;
		}

		debug::printf("\tSockets initialized!\n\n");

		freeaddrinfo(result);

		m_listening_thread = std::jthread(worker_StartListening);

		return 0;

		//--------------------------------------------------------------------------------------------------------------

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

	int worker_StartListening(const std::stop_token& stop_token) {
		debug::printf("Starting listening...\n");

		debug::printf("\tlisten()...\n");
		if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
			debug::printf("\t\tListen failed with error: %ld\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
		}

		debug::printf("\taccept()...\n");
		ClientSocket = accept(ListenSocket, nullptr, nullptr);
		if (ClientSocket == INVALID_SOCKET) {
			debug::printf("\t\taccept failed: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
		}

		debug::printf("\tConnection Accepted!\n\n");

		closesocket(ListenSocket);

		m_receiving_thread = std::jthread(worker_Receiver);

		return 0;
	}


	int worker_Receiver(const std::stop_token& stop_token) {
		debug::printf("Receiver loop...\n");
		#define DEFAULT_BUFLEN 512

		int8_t ReceiverBuffer[DEFAULT_BUFLEN];
		int iResult;
		int iSendResult;

		// Receive until the peer shuts down the connection
		do {
			if(stop_token.stop_requested()) {
				debug::printf("\tStopping worker thread for a Receiver\n");

				goto shutdown;

				return 0;
			}

			debug::printf("\trecv...\n");

			iResult = recv(ClientSocket, (char*)ReceiverBuffer, DEFAULT_BUFLEN, 0);
			if (iResult > 0)
			{
				debug::printf("\t\tBytes received: %d\n", iResult);

				ProcessResult(ReceiverBuffer);

				// Echo the buffer back to the sender
				iSendResult = send(ClientSocket, (char*)ReceiverBuffer, iResult, 0);
				if (iSendResult == SOCKET_ERROR) {
					debug::printf("\t\tsend failed: %d\n", WSAGetLastError());

					closesocket(ClientSocket);
					WSACleanup();

					return 1;
				}
				debug::printf("\t\tBytes sent: %d\n", iSendResult);
			}
			else if (iResult == 0)
			{
				debug::printf("\tConnection closing...\n");
			}
			else
			{
				debug::printf("\trecv failed: %d\n", WSAGetLastError());

				closesocket(ClientSocket);
				WSACleanup();

				return 1;
			}

		} while (iResult > 0);

		shutdown:{ // shutdown the send half of the connection since no more data will be sent
			iResult = shutdown(ClientSocket, SD_SEND);
			if (iResult == SOCKET_ERROR) {
				debug::printf("\tShutdown failed: %d\n", WSAGetLastError());

				closesocket(ClientSocket);
				WSACleanup();

				return 1;
			}

			closesocket(ClientSocket);
			WSACleanup();
		}

		debug::printf("Receiver has stopped!\n\n");

		return 0;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void ProcessResult(int8_t* bytes) {
		OnMessageReceived.Broadcast(bytes);
	}
}
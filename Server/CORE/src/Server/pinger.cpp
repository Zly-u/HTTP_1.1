#include "pinger.h"

#include "server.h"

#include <winsock2.h>
#include <ws2tcpip.h>

#include <thread>

#include "Shared/helpers.h"

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_BUF_LEN 512

namespace pinger {
	std::jthread m_listening_thread;
	std::jthread m_pinging_thread;

	// WSADATA WSA_Data;
	SOCKET ListenPingSocket = INVALID_SOCKET;
	SOCKET ClientPingSocket = INVALID_SOCKET;

	int StartPingerServer(){
		debug::printf("Starting Pinger...\n");

		addrinfo* result = nullptr;
		addrinfo hints{};
		int iResult = 0;

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_socktype = SOCK_DGRAM;
		hints.ai_family = AF_INET;
		hints.ai_flags = AI_PASSIVE;

		ListenPingSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (ListenPingSocket == INVALID_SOCKET) {
			printf("socket failed with error %d\n", WSAGetLastError());
			return 1;
		}

		//--------------------------------------------------------------------------------------------------------------

		sockaddr_in serverAddr{};

		serverAddr.sin_family = AF_INET;
		serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
		serverAddr.sin_port = htons(8080);
		if (bind(ListenPingSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr))) {
			printf("bind failed with error %d\n", WSAGetLastError());
			return 1;
		}

		//--------------------------------------------------------------------------------------------------------------

		int bytes_received;
		char serverBuf[1];
		int serverBufLen = 1;

		// Keep a seperate address struct to store sender information.
		sockaddr_in SenderAddr;
		int SenderAddrSize = sizeof (SenderAddr);

		printf("Receiving datagrams on %s\n", "127.0.0.1");
		bytes_received = recvfrom(
			ListenPingSocket,
			serverBuf, serverBufLen,
			0 /* no flags*/,
			(SOCKADDR*) &SenderAddr, &SenderAddrSize
		);
		if (bytes_received == SOCKET_ERROR) {
			printf("recvfrom failed with error %d\n", WSAGetLastError());
		}
		serverBuf[bytes_received] = '\0';

		//--------------------------------------------------------------------------------------------------------------

		m_listening_thread = std::jthread(worker_StartPinging);

		return 0;

		//--------------------------------------------------------------------------------------------------------------

		{
			listening_error:{
				closesocket(ListenPingSocket);
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

	int ShutdownPingerServer() {
		if (ClientSocket == INVALID_SOCKET) {
			return 1;
		}

		int error = 0;
		if (shutdown(ClientSocket, SD_SEND) == SOCKET_ERROR) {
			debug::printf("\tShutdown failed: %d\n", WSAGetLastError());
			error = 1;
		}

		closesocket(ClientSocket);
		WSACleanup();

		debug::printf("Pinger has stopped!\n\n");

		return error;
	}

	int worker_StartPinging(const std::stop_token& stop_token) {
		debug::printf("Starting listening...\n");

		debug::printf("\tlisten()...\n");
		if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
			debug::printf("\t\tListen failed with error: %ld\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
		}

		debug::printf("\taccept()...\n");
		sockaddr socket_address{};
		ClientSocket = accept(ListenSocket, &socket_address, nullptr);
		if (ClientSocket == INVALID_SOCKET) {
			debug::printf("\t\taccept failed: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
		}

		debug::printf("\tConnection Accepted!\n\n");

		char ClientName[DEFAULT_BUF_LEN];
		if(Receive(ClientName))
		{
			debug::printf("\tCouldn't receive Client's name.\n\n");
		}
		else
		{
			new_client.name = ClientName;
		}

		closesocket(ListenSocket);

		m_receiving_thread = std::jthread(worker_Receiver);

		return 0;
	}

	int Receive(char* return_buffer) {
		if (ClientSocket == INVALID_SOCKET) {
			return 1;
		}

		return recv(ClientSocket, return_buffer, DEFAULT_BUF_LEN, 0);
	}

	int worker_Pinger(const std::stop_token& stop_token) {
		debug::printf("Receiver loop...\n");

		int iResult;

		// Receive until the peer shuts down the connection
		do {
			if(stop_token.stop_requested()) {
				debug::printf("\tStopping worker thread for a Receiver\n");

				return 0;
			}

			debug::printf("\trecv...\n");

			iResult = Receive((char*)ReceiverBuffer);
			if (iResult > 0)
			{
				debug::printf("\t\tBytes received: %d\n", iResult);

				ProcessResult(ReceiverBuffer);
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

		debug::printf("Receiver has stopped!\n\n");

		return ShutdownServer(); // shutdown the send half of the connection since no more data will be sent
	}
}
#include "server.h"

#include <winsock2.h>
#include <ws2tcpip.h>

#include <thread>

#include "Shared/helpers.h"

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_BUF_LEN 512

namespace server {
	struct client_data {
		std::string name = "[PLACEHOLDER CLIENT]";
		std::string ip;

		std::jthread* ptr_thread = nullptr;
	};
	std::vector<client_data> clients;

	std::jthread m_listening_thread;
	std::jthread m_receiving_thread;

	int8_t ReceiverBuffer[DEFAULT_BUF_LEN];

	WSADATA WSA_Data;
	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	int StartServer(){
		debug::printf("Starting Server...\n");

		addrinfo* result = nullptr;
		addrinfo hints{};
		int iResult = 0;

		debug::printf("\tWSAStartup...\n");
		iResult = WSAStartup(MAKEWORD(2,2), &WSA_Data);
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

		OnServerStarted.Broadcast();

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

		OnServerListening.Broadcast();

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

		client_data new_client;
		char ClientName[DEFAULT_BUF_LEN];
		if(Receive(ClientName))
		{
			debug::printf("\tCouldn't receive Client's name.\n\n");
		}
		else
		{
			new_client.name = ClientName;
		}
		OnClientConnected.Broadcast(ClientName);
		clients.emplace_back(new_client);

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

	int worker_Receiver(const std::stop_token& stop_token) {
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

	int ShutdownServer() {
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

		debug::printf("Server has stopped!\n\n");

		OnServerShutdown.Broadcast();

		return error;
	}

	int SendCommandToAll(int8_t command, std::string&& data) {
		char* command_msg = new char[data.size() + 2];
		command_msg[0] = command;
		strcpy_s(command_msg+1, data.size()+1, data.c_str());

		const int res = SendToAll((int8_t*)command_msg);
		delete[] command_msg;

		return res;
	}

	int SendToAll(int8_t* data) {
		if(ClientSocket == INVALID_SOCKET) { return 1; }

		if (send(ClientSocket, (char*)data, (int) strlen((char*)data), 0) == SOCKET_ERROR) {
			debug::printf("\t\tsend failed: %d\n", WSAGetLastError());
			return 1;
		}

		debug::printf("\t\tBytes sent: %d\n", strlen((char*)data));

		return 0;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void ProcessResult(int8_t* bytes) {
		if (bytes[0] == 0x7F) {
			OnClientDisconnected.Broadcast((char*)bytes+1);
			return;
		}

		OnMessageReceived.Broadcast(bytes);
	}
}
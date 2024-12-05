#include "client.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include <cstdio>
#include <thread>

#include "Shared/helpers.h"

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")

#define DEFAULT_BUF_LEN 512

namespace client {
	std::jthread m_receiving_thread;

	char ReceiveBuffer[DEFAULT_BUF_LEN];

	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	int StartClient(char* new_client_name){
		debug::printf("Creating Client Socket...\n");

		addrinfo* result = nullptr;
		addrinfo* ptr = nullptr;
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

		debug::printf("\tgetaddrinfo...\n");
		// Resolve the local address and port to be used by the server
		if (getaddrinfo("127.0.0.1", "8080", &hints, &result)) {
			debug::printf("\t\tgetaddrinfo failed: %d\n", iResult);
			goto WSAStartup_error;
		}

		debug::printf("\tsocket()...\n");
		ConnectSocket = socket(
			result->ai_family,
			result->ai_socktype,
			result->ai_protocol
		);

		if (ConnectSocket == INVALID_SOCKET) {
			debug::printf("\t\tError at socket(): %ld\n", WSAGetLastError());
			goto socket_error;
		}

		// Connect to server.
		if (connect(ConnectSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
			debug::printf("\t\tUnable to connect to server!\n");
			goto connect_error;
		}

		freeaddrinfo(result);

		debug::printf("\tConnected to the server!\n\n");

		SendData((int8_t*)new_client_name);
		ClientName = new_client_name;

		m_receiving_thread = std::jthread(worker_Receiver);

		return 0;

		{
			connect_error:{
				closesocket(ConnectSocket);
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

	int SendData(int8_t* data) {
		if (ConnectSocket == INVALID_SOCKET) {
			return 1;
		}

		int iResult = send(ConnectSocket, (char*)data, (int)strlen((char*)data), 0);
		if (iResult == SOCKET_ERROR) {
			debug::printf("Data send failed with error: %d\n", WSAGetLastError());

			closesocket(ConnectSocket);
			WSACleanup();

			return 1;
		}

		debug::printf("Bytes Sent: %ld\n", iResult);

		return 0;
	}

	int worker_Receiver(const std::stop_token& stop_token) {
		if(stop_token.stop_requested()) {
			return 0;
		}

		int iResult;
		// Receive until the peer closes the connection
		do {
			iResult = recv(ConnectSocket, ReceiveBuffer, DEFAULT_BUF_LEN, 0);

			if(iResult > 0)
			{
				debug::printf("\tBytes received: %d\n", iResult);
				ProcessResult((int8_t*)ReceiveBuffer);
			}
			else if(iResult == 0)
			{
				debug::printf("\tConnection closed\n");
				OnServerShutdown.Broadcast();
			}
			else
			{
				debug::printf("\trecv() failed with error: %d\n", WSAGetLastError());
				return 1;
			}

		} while(iResult > 0);

		return 0;
	}


	int ClientShutdown() {
		debug::printf("Shutting down Client Socket");

		if (m_receiving_thread.joinable()) {
			debug::printf("\tStopping thread.");
			m_receiving_thread.request_stop();
			m_receiving_thread.join();
			debug::printf("\tThread is stopped.");
		}

		//Shutdown message to the server.
		SendCommand(0x7F, ClientName);

		if (shutdown(ConnectSocket, SD_SEND) == SOCKET_ERROR) {
			debug::printf("\t\tShutdown failed with error: %d\n", WSAGetLastError());

			closesocket(ConnectSocket);
			WSACleanup();

			return 1;
		}

		closesocket(ConnectSocket);
		WSACleanup();

		return 0;
	}

	int SendCommand(int8_t command, std::string& data) {
		char* command_msg = new char[data.size() + 2];
		command_msg[0] = command;
		strcpy_s(command_msg+1, data.size()+1, data.c_str());

		const int res = SendData((int8_t*)command_msg);
		delete[] command_msg;

		return res;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void ProcessResult(int8_t* bytes) {
		if (bytes[0] == 0x7F) {
			OnServerKick.Broadcast((char*)bytes+1);
			ClientShutdown();
			return;
		}
		OnClientMessageReceived.Broadcast(bytes);
	}
}
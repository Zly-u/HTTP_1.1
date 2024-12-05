#pragma once

#include <cstdint>
#include <stop_token>
#include <string>

#include "Shared/Delegates.h"

DECLARE_MULTICAST_DELEGATE(Delegate_OnServerStarted);
DECLARE_MULTICAST_DELEGATE(Delegate_OnServerListening);
DECLARE_MULTICAST_DELEGATE(Delegate_OnServerShutdown);

DECLARE_MULTICAST_DELEGATE(Delegate_OnClientConnected, char*);
DECLARE_MULTICAST_DELEGATE(Delegate_OnClientDisconnected, char*);

DECLARE_MULTICAST_DELEGATE(Delegate_OnMessageReceived, int8_t*);

namespace server {
	inline Delegate_OnMessageReceived    OnMessageReceived;
	inline Delegate_OnServerStarted      OnServerStarted;
	inline Delegate_OnServerListening    OnServerListening;
	inline Delegate_OnServerShutdown     OnServerShutdown;
	inline Delegate_OnClientConnected    OnClientConnected;
	inline Delegate_OnClientDisconnected OnClientDisconnected;

	int StartServer();
	int ShutdownServer();
	int Receive(char* return_buffer);
	int worker_StartListening(const std::stop_token& stop_token);
	int worker_Receiver(const std::stop_token& stop_token);

	int SendCommandToAll(int8_t command, std::string&& data);
	int SendToAll(int8_t* data);
	void ProcessResult(int8_t* bytes);
}

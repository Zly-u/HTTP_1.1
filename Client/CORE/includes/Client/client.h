#pragma once

#include <cstdint>
#include <stop_token>
#include <string>

#include "Shared/Delegates.h"

DECLARE_MULTICAST_DELEGATE(Delegate_OnConnected);
DECLARE_MULTICAST_DELEGATE(Delegate_OnDisconnected);
DECLARE_MULTICAST_DELEGATE(Delegate_OnOtherClientConnected, char*);

DECLARE_MULTICAST_DELEGATE(Delegate_OnServerShutdown);
DECLARE_MULTICAST_DELEGATE(Delegate_OnServerKick, char*);

DECLARE_MULTICAST_DELEGATE(Delegate_OnClientMessageReceived, int8_t*);

namespace client {
	inline std::string              ClientName;
	inline std::vector<std::string> ClientsInServer;

	// TODO: OnConnected
	inline Delegate_OnConnected OnConnected;
	// TODO: OnDisconnected
	inline Delegate_OnDisconnected OnDisconnected;
	inline Delegate_OnServerShutdown OnServerShutdown;
	// TODO: OnServerKick
	inline Delegate_OnServerKick OnServerKick;
	// TODO: OnOtherClientConnected
	inline Delegate_OnOtherClientConnected OnOtherClientConnected;
	inline Delegate_OnClientMessageReceived OnClientMessageReceived;

	int StartClient(char* new_client_name);
	int SendCommand(int8_t command, std::string& data);
	int SendData(int8_t* data);
	int worker_Receiver(const std::stop_token& stop_token);
	int ClientShutdown();

	void ProcessResult(int8_t* bytes);
}

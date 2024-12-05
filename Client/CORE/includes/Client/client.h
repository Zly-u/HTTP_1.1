#pragma once

#include <cstdint>
#include <stop_token>

#include "Shared/Delegates.h"

DECLARE_MULTICAST_DELEGATE(Delegate_OnClientMessageReceived, int8_t*);

namespace client {
	inline Delegate_OnClientMessageReceived OnClientMessageReceived;

	int StartClient();
	int SendData(int8_t* data);
	int worker_Receiver(const std::stop_token& stop_token);
	int ClientShutdown();

	void ProcessResult(int8_t* bytes);
}

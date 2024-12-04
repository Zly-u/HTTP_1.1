#pragma once

#include <cstdint>
#include <stop_token>

#include "Shared/Delegates.h"

DECLARE_MULTICAST_DELEGATE(Delegate_OnMessageReceived, int8_t*);

namespace server {
	inline Delegate_OnMessageReceived OnMessageReceived;

	int StartServer();
	int worker_StartListening(const std::stop_token& stop_token);
	int worker_Receiver(const std::stop_token& stop_token);

	void ProcessResult(int8_t* bytes);
}

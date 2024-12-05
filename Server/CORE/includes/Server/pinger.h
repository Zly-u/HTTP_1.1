#pragma once

#include <cstdint>
#include <stop_token>
#include <string>

#include "Shared/Delegates.h"

namespace pinger {
	int StartPingerServer();
	int ShutdownPingerServer();
	int Receive(char* return_buffer);
	int worker_StartPinging(const std::stop_token& stop_token);
	int worker_Pinger(const std::stop_token& stop_token);
}

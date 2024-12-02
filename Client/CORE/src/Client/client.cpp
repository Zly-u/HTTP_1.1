#include "client.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include <cstdio>

#include "Shared/helpers.h"

#pragma comment(lib, "Ws2_32.lib")

namespace client {
	int StartClient(){
		debug::printf("Testing connection to server\n");

		return 1;
	}
}
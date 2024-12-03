#pragma once
#include <string>

#include "Shared/Delegate.h"

DECLARE_MULTICAST_DELEGATE(OnSend, char*);

class GUI_History {
public:
	static void ShowHistoryWindow();
	static void ShowClientsWindow();


	static char input_buffer[128];
	static std::string input_command;

	static OnSend OnSendEvent;
};

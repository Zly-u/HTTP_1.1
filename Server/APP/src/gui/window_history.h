#pragma once
#include <string>

#include "Shared/Delegates.h"

DECLARE_MULTICAST_DELEGATE(OnSend, char*);

class GUI_History {
public:
	static void AddServerMessage(char* message);
	static void AddClientMessage(char* message);

	static void ShowHistoryWindow();
	static void ShowClientsWindow();

	inline static OnSend OnSendEvent;

private:
	inline static std::vector<std::string> m_history;
	inline static char m_input_buffer[128] = "";
	inline static std::string m_input_command;
};

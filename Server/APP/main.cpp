#include <print>

#include "gui/GUI_Server.h"

#include "Shared/App.h"
#include "Shared/MainWindow.h"

int wmain(int /*argc*/, wchar_t** /*argv*/){
	AppSettings as;
	as.ini_name = "ServerConfig.ini";

	WindowSettings ws;
	ws.Title = "HTTP Server";

	GUI_Server gui;

	App app(as, ws, gui);
	app.Run();

	return 0;
}

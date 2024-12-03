#include "GUI_Client.h"

#include "Shared/App.h"
#include "Shared/MainWindow.h"

int wmain(int /*argc*/, wchar_t** /*argv*/){
	AppSettings as;
	as.ini_name = "ClientConfig.ini";

	WindowSettings ws;
	ws.Title = "HTTP Client";

	GUI_Client gui;

	App app(as, ws, gui);
	app.Run();

    return 0;
}

#include <vector>

#include "GUI_Client.h"

#include "Shared/App.h"
#include "Shared/MainWindow.h"


int GetCores() {
	return 8;
}


int wmain(int /*argc*/, wchar_t** /*argv*/){
	AppSettings as;
	WindowSettings ws;
	GUI_Client gui;

	App app(as, ws, gui);
	app.Run();

    return 0;
}

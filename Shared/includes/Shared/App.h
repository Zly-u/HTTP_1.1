﻿#pragma once

#include "library.h"

#include <string>

struct AppSettings {
	std::string title;
	std::string ini_name;
};

class SHARED_API App{
public:
	App(AppSettings& as, struct WindowSettings& ws, class Interface_Gui& gui);
	~App();

	void Run();
	void DrawGui();

private:
	AppSettings* m_settings;
	Interface_Gui* m_gui;
};
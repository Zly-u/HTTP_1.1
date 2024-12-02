#pragma once

#include "Shared/Interface_Gui.h"
#include "Shared/imgui/imgui.h"

struct SDL_Texture;
struct SDL_Renderer;

class GUI_Client : public Interface_Gui {
public:
	~GUI_Client(){
		CleanUp();
	}

	// Interface_Gui
	void Init(SDL_Renderer* renderer) override;
	void SetupDocking() override;
	void Draw() override;
	void Render() override;
	void CleanUp() override;
	// ~Interface_Gui

private:
	/// UI ELEMENTS ///
	void UI_ShowMenu_File_Impl();
	void HelpMarker_Impl(const char* desc);


private:
	bool dock_is_open	= true;
	bool opt_fullscreen	= true;
	bool opt_padding	= false; // Removes padding for all the child windows

	// Our state
	bool   show_demo_window    = true;
	bool   show_another_window = false;
	ImVec4 clear_color         = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
};


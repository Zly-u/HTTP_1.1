#pragma once

#include "Shared/Interface_Gui.h"
#include "Shared/imgui/imgui.h"

struct SDL_Texture;
struct SDL_Renderer;

class GUI_Client final : public Interface_Gui {
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

	const ImColor& GetClearColor() const override { return m_clear_color; };
	// ~Interface_Gui

private:
	/// UI ELEMENTS ///
	void UI_ShowMenu_File_Impl();
	void HelpMarker_Impl(const char* desc);


private:
	bool m_dock_is_open   = true;
	bool m_opt_fullscreen = true;
	bool m_opt_padding    = false; // Removes padding for all the child windows

	// Our state
	bool   m_show_demo_window    = true;
	bool   m_show_another_window = false;
	ImColor m_clear_color        = ImColor(0.45f, 0.55f, 0.60f, 1.00f);
};


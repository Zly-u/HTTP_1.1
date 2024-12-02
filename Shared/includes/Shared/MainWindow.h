#pragma once

#include "library.h"

#include <string>

#include "helpers.h"
#include "Shared/imgui/imgui_impl_sdlrenderer2.h"
#include "Shared/imgui/imgui_impl_sdl2.h"

#include "Shared/SDL/include/SDL.h"

struct SHARED_API WindowSettings
{
	WindowSettings() {
		SDL_DisplayMode DesktopDisplayMode;
		SDL_GetDesktopDisplayMode(0, &DesktopDisplayMode);
		// width  = DesktopDisplayMode.w * percent_of_the_screen; // NOLINT(bugprone-narrowing-conversions)
		// height = DesktopDisplayMode.h * percent_of_the_screen; // NOLINT(bugprone-narrowing-conversions)
	}

	std::string Title = "[TITLE PLACEHOLDER]";

	float percent_of_the_screen = 0.7f;
	int width = 1280;
	int height = 720;

	SDL_WindowFlags window_flags = static_cast<SDL_WindowFlags>(
		0
		| SDL_WINDOW_RESIZABLE
		| SDL_WINDOW_ALLOW_HIGHDPI
		| SDL_WINDOW_SHOWN
	);

	ImGuiConfigFlags ConfigFlags = ImGuiConfigFlags_None
		| ImGuiConfigFlags_NavEnableKeyboard	// Enable Keyboard Controls
		| ImGuiConfigFlags_DockingEnable        // Enable Docking
		| ImGuiConfigFlags_ViewportsEnable      // Enable Multi-Viewport / Platform Windows
	// | ImGuiConfigFlags_ViewportsNoTaskBarIcons;
	// | ImGuiConfigFlags_ViewportsNoMerge;
	;
};

class SHARED_API MainWindow : Singleton<MainWindow>{
public:
	MainWindow() :
		SDL_Window_ptr(nullptr),
		ImIO_ptr(nullptr)
	{}

	~MainWindow() {
		CleanUp_Impl();
	}

	static int WindowSetup(const WindowSettings& ws) {
		return Get().WindowSetup_Impl(ws);
	}

	static void CleanUp() {
    	return Get().CleanUp_Impl();
    }

	// SDL Getters

	static SDL_Renderer* Get_SDLRenderer() {
		return Get().SDL_Renderer_ptr;
	}
	static SDL_Window* Get_SDLWindow() {
		return Get().SDL_Window_ptr;
	}

	// ImGUI Getters

	static ImGuiContext* Get_ImGuiContext() {
		return Get().context;
	}

	static ImGuiIO* Get_ImGuiIO() {
		return Get().ImIO_ptr;
	}
	
private:
	int  WindowSetup_Impl(const WindowSettings& ws);
	void CleanUp_Impl();


private:
	// SDL
	SDL_Renderer*	SDL_Renderer_ptr= nullptr;
	SDL_Window*		SDL_Window_ptr	= nullptr;

	// ImGui
	ImGuiContext* context  = nullptr;
	ImGuiIO*      ImIO_ptr = nullptr;
};

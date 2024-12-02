﻿#include <print>

#include "MainWindow.h"

#include "Shared/SDL_image/include/SDL_image.h"

int MainWindow::WindowSetup_Impl(const WindowSettings& ws){
	constexpr Uint32 SDL_INIT_FLAGS = 0
		| SDL_INIT_VIDEO
		| SDL_INIT_TIMER
		| SDL_INIT_EVENTS
	;
	if (SDL_Init(SDL_INIT_FLAGS) != 0) { // 0 - success
		std::println("SDL Init Error: {}", SDL_GetError());
		return -1;
	}
	constexpr Uint32 SDL_IMAGE_INIT_FLAGS = 0
		| IMG_INIT_PNG
		| IMG_INIT_JPG
	;
	if (IMG_Init(SDL_IMAGE_INIT_FLAGS) == 0) { // 0 - fail
		std::println("SDL Init Error: {}", SDL_GetError());
		return -1;
	}

	// From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
	SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

	// Create window with SDL_Renderer graphics context
	SDL_Window_ptr = SDL_CreateWindow(
		ws.Title.c_str(),
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		ws.width, ws.height,
		ws.window_flags
	);
	if (!SDL_Window_ptr) {
		std::println("Error: SDL_CreateWindow(): {}", SDL_GetError());
		return -1;
	}

	constexpr Uint32 MAIN_RENDERER_FLAGS = SDL_RENDERER_ACCELERATED;
	SDL_Renderer_ptr = SDL_CreateRenderer(SDL_Window_ptr, -1, MAIN_RENDERER_FLAGS);
	if(!SDL_Renderer_ptr) {
		CleanUp_Impl();
		std::println("Error: SDL_CreateRenderer(): {}", SDL_GetError());
		return -1;
	}

	// Setup ImGui context
	IMGUI_CHECKVERSION();
	context = ImGui::CreateContext();
	ImIO_ptr = &ImGui::GetIO();
	ImIO_ptr->ConfigFlags |= 0
		| ImGuiConfigFlags_NavEnableKeyboard	// Enable Keyboard Controls
		| ImGuiConfigFlags_DockingEnable		// Enable Docking
		| ImGuiConfigFlags_ViewportsEnable		// Enable Multi-Viewport / Platform Windows
	;
	ImGui_ImplSDL2_InitForSDLRenderer(
		SDL_Window_ptr,
		SDL_Renderer_ptr
	);
	ImGui_ImplSDLRenderer2_Init(SDL_Renderer_ptr);
	
	return 0;
}

void MainWindow::CleanUp_Impl(){
	if(SDL_Renderer_ptr) {
		// TODO: Freezes the app whe built Dynamically.
		SDL_DestroyRenderer(SDL_Renderer_ptr);
	}
	if(SDL_Window_ptr) {
		SDL_DestroyWindow(SDL_Window_ptr);
	}
	SDL_Quit();
}


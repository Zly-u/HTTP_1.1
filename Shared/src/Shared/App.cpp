#include "App.h"

#include "Shared/MainWindow.h"
#include "Shared/IniManager.h"

#include "Interface_Gui.h"

App::App(AppSettings& as, WindowSettings& ws, Interface_Gui& gui) : m_settings(&as), m_gui(&gui) {
	debug::printf("Starting App\n");

	if(MainWindow::WindowSetup(ws) != 0){
		debug::printf("Window Setup Failed\n");
		return;
	}

	// Creates an Ini file if it doesn't exist in the root dir of the app.
	IniManager::LoadFile(m_settings->ini_name);

	m_gui->Init(MainWindow::Get_SDLRenderer());
}


App::~App() {
	debug::printf("\nApp::App CLEANING UP\n\n");
}


void App::Run() {
	bool done = false;
    while (!done) {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true,    do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    	SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);

        	switch(event.type) {
        		case SDL_QUIT:
        			done = true;
        			break;
		        default:
        			break;
	        }

        	if (event.type == SDL_WINDOWEVENT &&
        		event.window.event == SDL_WINDOWEVENT_CLOSE &&
        		event.window.windowID == SDL_GetWindowID(MainWindow::Get_SDLWindow())
        	){
        		done = true;
        	}
        }

    	// Start the ImGui frame
		ImGui_ImplSDLRenderer2_NewFrame();
    	ImGui_ImplSDL2_NewFrame();
    	ImGui::NewFrame();

    	DrawGui();

    	// End frame
    	SDL_RenderSetScale(
    		MainWindow::Get_SDLRenderer(),
    		MainWindow::Get_ImGuiIO()->DisplayFramebufferScale.x,
    		MainWindow::Get_ImGuiIO()->DisplayFramebufferScale.y
    	);
    	const ImVec4& clr_color = GetGui().GetClearColor().Value;
    	SDL_SetRenderDrawColor(
    		MainWindow::Get_SDLRenderer(),
    		static_cast<uint8_t>(clr_color.x * 255),
    		static_cast<uint8_t>(clr_color.y * 255),
    		static_cast<uint8_t>(clr_color.z * 255),
    		static_cast<uint8_t>(clr_color.w * 255)
    	);
    	SDL_RenderClear(MainWindow::Get_SDLRenderer());
    	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), MainWindow::Get_SDLRenderer());

    	// Display all drawn contents from the backbuffer
    	SDL_RenderPresent(MainWindow::Get_SDLRenderer());

    	SDL_Delay(16);
    }
}

void App::DrawGui() {
	m_gui->Draw();
	m_gui->Render();
}
#include "window_history.h"

#include "server.h"
#include "Shared/imgui/imgui.h"

void GUI_History::AddServerMessage(char* message) {
	m_history.emplace_back("[Server]:" + std::string(message));
}

void GUI_History::AddClientMessage(char* message) {
	m_history.emplace_back("[Client]:" + std::string(message));
}

void GUI_History::ShowHistoryWindow(){
	static bool show = true;
	constexpr ImGuiWindowFlags h_wf = ImGuiWindowFlags_None
		| ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoScrollbar
	;

	if (ImGui::Begin("History"), show, h_wf) {
		constexpr ImGuiChildFlags wf = ImGuiChildFlags_None
		// | ImGuiChildFlags_AlwaysAutoResize
		// | ImGuiChildFlags_AutoResizeY
		| ImGuiChildFlags_FrameStyle
		;

		if (ImGui::BeginChild(69, ImVec2(-1, ImGui::GetWindowHeight() - 58), wf)) {
			for(auto& messsage : m_history) {
				ImGui::Text(messsage.c_str());
			}
		}ImGui::EndChild();

		////////////////////////////////////////////////////////////////////////////////////

		constexpr ImGuiInputTextFlags f_it = ImGuiInputTextFlags_None;

		const float butt_width = ImGui::CalcItemWidth();
		if (ImGui::Button("Start Host")) {
			if(server::StartServer() == 0) {
			}
		}

		ImGui::SameLine();

		static char c_input_buffer[128];
		ImGui::PushItemWidth(ImGui::GetWindowWidth() - butt_width);
		ImGui::InputText("<<", c_input_buffer, 128, f_it);
		ImGui::PopItemWidth();

		ImGui::SameLine();

		if (ImGui::Button("Send")) {
			OnSendEvent.Broadcast(c_input_buffer);
			strcpy_s(c_input_buffer, 1, "");
		}
	} ImGui::End();
}

void GUI_History::ShowClientsWindow() {
	if (ImGui::Begin("Clients")) {

	} ImGui::End();
}

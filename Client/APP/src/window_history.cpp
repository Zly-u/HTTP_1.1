#include "window_history.h"

#include "client.h"
#include "Shared/imgui/imgui.h"

void GUI_History::AddMessage(std::string&& message) {
	m_history.emplace_back(std::move(message));
}

void GUI_History::AddServerMessage(char* message) {
	AddMessage("[Server]:" + std::string(message));
}

void GUI_History::AddClientMessage(char* message) {
	AddMessage("[Client]:" + std::string(message));
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
			for(auto& message : m_history) {
				ImGui::Text(message.c_str());
			}
		}ImGui::EndChild();

		////////////////////////////////////////////////////////////////////////////////////

		constexpr ImGuiInputTextFlags f_it = ImGuiInputTextFlags_None;
		static char c_name_buffer[16];
		const float butt_width = ImGui::CalcItemWidth();
		if (ImGui::Button("Connect")) {
			if (strlen(c_name_buffer) != 0) {
				if(client::StartClient(c_name_buffer) == 0) {
				}
			}
		}

		ImGui::SameLine();

		ImGui::PushItemWidth(60);
		ImGui::InputTextWithHint("##", "Your name", c_name_buffer, 128, f_it);
		ImGui::PopItemWidth();

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

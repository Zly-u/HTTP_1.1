#include "window_history.h"

#include "Shared/imgui/imgui.h"

char GUI_History::input_buffer[128] = "";
OnSend GUI_History::OnSendEvent;

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

		}ImGui::EndChild();

		////////////////////////////////////////////////////////////////////////////////////

		constexpr ImGuiInputTextFlags f_it = ImGuiInputTextFlags_None;

		static char c_input_buffer[128];
		ImGui::PushItemWidth(ImGui::GetWindowWidth() - 83);
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

#include "ui.h"
#include "imgui/imgui.h"
#include "math.h"
#include "stdio.h"


int SIDEBAR_WIDTH = 220;

void drawSideBar(NodeAddState* isAdding, int* c) {
	ImVec2 windowSize = ImGui::GetMainViewport()->Size;
	ImGui::SetNextWindowSize({( float )SIDEBAR_WIDTH, windowSize.y * 0.1f});
	ImGui::SetNextWindowPos({( float )windowSize.x - SIDEBAR_WIDTH, 0});

	ImGui::Begin(
		"Desc",
		nullptr,
		ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
	ImGui::TextUnformatted("Red Color is Logic Low");
	ImGui::TextUnformatted("Green Color is Logic High");
	ImGui::End();

	ImGui::SetNextWindowSize({( float )SIDEBAR_WIDTH, windowSize.y * 0.9f});
	ImGui::SetNextWindowPos({windowSize.x - SIDEBAR_WIDTH, 0.1f * windowSize.y});

	ImGui::Begin(
		"Node Adders",
		nullptr,
		ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
	ImGui::TextUnformatted("Add Nodes!");

	ImGui::NewLine();
	ImVec2 size = ImVec2(125, 20);
	ImGui::Button("Input", size);
	if (ImGui::IsItemClicked())
		*isAdding = {true, INPUT};
	char temp[24];
	if (*c == INPUT) {
		for (int i = 0; i < 8; i++) {
			snprintf(temp, sizeof(temp), "%d", i + 1);
			ImGui::Button(temp, size);
			if (ImGui::IsItemClicked()) {
				*isAdding = {true, NodeType(INPUT_1 + i)}, *c = false;
			}
		}
	}
	ImGui::Button("Output", size);
	if (ImGui::IsItemClicked()) {
		*isAdding = {true, OUTPUT};
	}

	ImGui::Button("And Gate", size);
	if (ImGui::IsItemClicked()) {
		*isAdding = {true, AND_GATE};
	}
	if (*c == AND_GATE) {
		for (int i = 0; i < 7; i++) {
			snprintf(temp, sizeof(temp), "%d %s", i + 2, "input");
			ImGui::Button(temp, size);
			if (ImGui::IsItemClicked()) {
				*isAdding = {true, NodeType(AND_GATE_2 + i)}, *c = false;
			}
		}
	}

	ImGui::Button("Or Gate", size);
	if (ImGui::IsItemClicked()) {
		*isAdding = {true, OR_GATE};
	}
	if (*c == OR_GATE) {
		for (int i = 0; i < 7; i++) {
			snprintf(temp, sizeof(temp), "%d %s", i + 2, "input");
			ImGui::Button(temp, size);
			if (ImGui::IsItemClicked()) {
				*isAdding = {true, NodeType(OR_GATE_2 + i)}, *c = false;
			}
		}
	}

	ImGui::Button("Not Gate", size);
	if (ImGui::IsItemClicked()) {
		*isAdding = {true, NOT_GATE};
	}

	ImGui::Button("Encoder", size);
	if (ImGui::IsItemClicked()) {
		*isAdding = {true, ENCODER};
	}
	if (*c == ENCODER) {
		for (int i = 0; i < 3; i++) {
			snprintf(temp, sizeof(temp), "%d:%d %s", int(pow(2, 3 - i)), 3 - i, "Encoder");
			ImGui::Button(temp, size);
			if (ImGui::IsItemClicked()) {
				*isAdding = {true, NodeType(ENCODER8_3 + i)}, *c = false;
			}
		}
	}

	ImGui::Button("Decoder", size);
	if (ImGui::IsItemClicked()) {
		*isAdding = {true, DECODER};
	}
	if (*c == DECODER) {
		for (int i = 0; i < 3; i++) {
			snprintf(temp, sizeof(temp), "%d:%d %s", 3 - i, int(pow(2, 3 - i)), "Decoder");
			ImGui::Button(temp, size);
			if (ImGui::IsItemClicked()) {
				*isAdding = {true, NodeType(DECODER3_8 + i)}, *c = false;
			}
		}
	}


	ImGui::Button("MultiPlexer", size);
	if (ImGui::IsItemClicked()) {
		*isAdding = {true, MULTIPLEXER};
	}
	if (*c == MULTIPLEXER) {
		for (int i = 0; i < 3; i++) {
			snprintf(temp, sizeof(temp), "%d:%d %s", int(pow(2, 3 - i)), 1, "MUX");
			ImGui::Button(temp, size);
			if (ImGui::IsItemClicked()) {
				*isAdding = {true, NodeType(MULTIPLEXER8_1 + i)}, *c = false;
			}
		}
	}

	ImGui::Button("DeMultiPlexer", size);
	if (ImGui::IsItemClicked()) {
		*isAdding = {true, DEMULTIPLEXER};
	}
	if (*c == DEMULTIPLEXER) {
		for (int i = 0; i < 3; i++) {
			snprintf(temp, sizeof(temp), "%d:%d %s", 1, int(pow(2, 3 - i)), "DeMUX");
			ImGui::Button(temp, size);
			if (ImGui::IsItemClicked()) {
				*isAdding = {true, NodeType(DEMULTIPLEXER1_8 + i)}, *c = false;
			}
		}
	}

	ImGui::End();
}
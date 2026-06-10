#include "ui.h"
#include "imgui/imgui.h"
#include "imnodes/imnodes.h"
#include "math.h"
#include "stdio.h"


int SIDEBAR_WIDTH = 220;

void drawEditor(GraphicsContext* context, NodeManager* nodeManager, bool adding, int& uniqueNumber) {
	ImVec2 viewPortSize = ImGui::GetMainViewport()->Size;
	ImGui::SetNextWindowSize({viewPortSize.x - SIDEBAR_WIDTH, viewPortSize.y});
	ImGui::SetNextWindowPos({0, 0});

	ImGui::Begin(
		"Encloser",
		NULL,
		ImGuiWindowFlags_NoBringToFrontOnFocus | // Always want the Editor to be on background
			ImGuiWindowFlags_NoMove |			 // Always want the Editor to Stay on top right
			ImGuiWindowFlags_NoResize |			 // Always want it take all space (on background)
			ImGuiWindowFlags_NoTitleBar			 // Always want the title bar to never appear
	);
	ImNodes::EditorContextSet(context->editorContext);
	ImNodes::BeginNodeEditor();

	nodeManager->render();

	ImNodes::MiniMap();
	ImNodes::EndNodeEditor();

	int linkId = -1, startNodeId = -1, startPinId = -1, endNodeId = -1, endPinId = -1;
	bool created_from_snap;
	if (ImNodes::IsLinkCreated(&startNodeId, &startPinId, &endNodeId, &endPinId, &created_from_snap)) {
		uniqueNumber++;
		nodeManager->addLink(uniqueNumber, startNodeId, startPinId, endNodeId, endPinId);
	}
	if (ImNodes::IsLinkDestroyed(&linkId)) {
		nodeManager->removeLink(linkId);
	}

	ImGui::End();

	if (adding) {
		ImGui::SetNextWindowBgAlpha(0.40f);
		ImGui::Begin(
			"Adding Node",
			nullptr,
			ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_NoTitleBar);
		ImGui::End();
	}
}

void drawSideBar(NodeType* newNodeType) {
	ImVec2 windowSize = ImGui::GetMainViewport()->Size;
	{
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
	}

	ImGui::SetNextWindowSize({( float )SIDEBAR_WIDTH, windowSize.y * 0.9f});
	ImGui::SetNextWindowPos({windowSize.x - SIDEBAR_WIDTH, 0.1f * windowSize.y});

	ImGui::Begin(
		"Node Adders",
		nullptr,
		ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
	ImGui::TextUnformatted("Add Nodes!");

	ImGui::NewLine();
	char temp[24];
	ImVec2 size = ImVec2(125, 20);
	if (ImGui::TreeNode("Input")) {
		for (int i = 0; i < 8; i++) {
			snprintf(temp, sizeof(temp), "%d", i + 1);
			ImGui::Button(temp, size);
			if (ImGui::IsItemClicked()) {
				*newNodeType = NodeType(INPUT_1 + i);
			}
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Output")) {
		ImGui::Button("Output", size);
		if (ImGui::IsItemClicked()) {
			*newNodeType = OUTPUT;
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("And Gate")) {
		for (int i = 0; i < 7; i++) {
			snprintf(temp, sizeof(temp), "%d %s", i + 2, "input");
			ImGui::Button(temp, size);
			if (ImGui::IsItemClicked()) {
				*newNodeType = NodeType(AND_GATE_2 + i);
			}
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Or Gate")) {
		for (int i = 0; i < 7; i++) {
			snprintf(temp, sizeof(temp), "%d %s", i + 2, "input");
			ImGui::Button(temp, size);
			if (ImGui::IsItemClicked()) {
				*newNodeType = NodeType(OR_GATE_2 + i);
			}
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Not Gate")) {
		ImGui::Button("Not Gate", size);
		if (ImGui::IsItemClicked()) {
			*newNodeType = NOT_GATE;
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Encoder")) {
		for (int i = 0; i < 3; i++) {
			snprintf(temp, sizeof(temp), "%d:%d %s", int(pow(2, 3 - i)), 3 - i, "Encoder");
			ImGui::Button(temp, size);
			if (ImGui::IsItemClicked()) {
				*newNodeType = NodeType(ENCODER8_3 + i);
			}
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Decoder")) {
		for (int i = 0; i < 3; i++) {
			snprintf(temp, sizeof(temp), "%d:%d %s", 3 - i, int(pow(2, 3 - i)), "Decoder");
			ImGui::Button(temp, size);
			if (ImGui::IsItemClicked()) {
				*newNodeType = NodeType(DECODER3_8 + i);
			}
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("MultiPlexer")) {
		for (int i = 0; i < 3; i++) {
			snprintf(temp, sizeof(temp), "%d:%d %s", int(pow(2, 3 - i)), 1, "MUX");
			ImGui::Button(temp, size);
			if (ImGui::IsItemClicked()) {
				*newNodeType = NodeType(MULTIPLEXER8_1 + i);
			}
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("DeMultiPlexer")) {
		for (int i = 0; i < 3; i++) {
			snprintf(temp, sizeof(temp), "%d:%d %s", 1, int(pow(2, 3 - i)), "DeMUX");
			ImGui::Button(temp, size);
			if (ImGui::IsItemClicked()) {
				*newNodeType = NodeType(DEMULTIPLEXER1_8 + i);
			}
		}
		ImGui::TreePop();
	}

	ImGui::End();
}
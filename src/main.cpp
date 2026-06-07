#include <SDL3/SDL.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl3.h>
#include <imgui/imgui_impl_sdlgpu3.h>
#include <imnodes/imnodes.h>
#include <math.h>

#include "NodeAddState.h"
#include "NodeManager.h"
#include "setup.h"
#include "ui.h"


int main(int, char**) {
	GraphicsContext context = createWindow();

	bool done = false;
	int c	  = 0; // to identify proper generics for combinational circuit

	int uniqueNumber = 0;
	NodeManager nodeManager;

	NodeAddState isAdding = {false, NOT_ADDING};
	while (!done) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			ImGui_ImplSDL3_ProcessEvent(&event);
			if (event.type == SDL_EVENT_QUIT || (event.key.key == SDLK_ESCAPE)) {
				done = true;
			}
			if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED &&
				event.window.windowID == SDL_GetWindowID(context.window))
				done = true;
			if (isAdding.isAdding && isAdding.newNodeType > GENERICS) {
				c = isAdding.newNodeType;
			} else if (isAdding.isAdding && event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
				uniqueNumber++;
				ImNodes::SetNodeScreenSpacePos(uniqueNumber, ImGui::GetMousePos());
				nodeManager.addNode(uniqueNumber, "Add Node", isAdding.newNodeType);
				isAdding = {false, NOT_ADDING};
				c		 = NOT_ADDING;
			}
		}

		if (SDL_GetWindowFlags(context.window) & SDL_WINDOW_MINIMIZED) {
			SDL_Delay(10);
			continue;
		}

		// Start the Dear ImGui frame
		ImGui_ImplSDLGPU3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();
		ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));

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
		ImNodes::EditorContextSet(context.editorContext);
		ImNodes::BeginNodeEditor();

		nodeManager.render();

		ImNodes::MiniMap();
		ImNodes::EndNodeEditor();

		int linkId = -1, startNodeId = -1, startPinId = -1, endNodeId = -1, endPinId = -1;
		bool created_from_snap;
		if (ImNodes::IsLinkCreated(&startNodeId, &startPinId, &endNodeId, &endPinId, &created_from_snap)) {
			uniqueNumber++;
			nodeManager.addLink(uniqueNumber, startNodeId, startPinId, endNodeId, endPinId);
		}
		if (ImNodes::IsLinkDestroyed(&linkId)) {
			nodeManager.removeLink(linkId);
		}

		ImGui::End();

		if (isAdding.isAdding) {
			ImGui::SetNextWindowBgAlpha(0.40f);
			ImGui::Begin(
				"Adding Node",
				nullptr,
				ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
					ImGuiWindowFlags_NoTitleBar);
			ImGui::End();
		}

		drawSideBar(&isAdding, &c);

		ImGui::PopStyleVar();

		// ImGui::ShowDemoWindow();
		nodeManager.copyover();
		nodeManager.calculate();
		render(context);
	}

	destroyWindow(context);
	return 0;
}

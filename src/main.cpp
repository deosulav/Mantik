#include <SDL3/SDL.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl3.h>
#include <imgui/imgui_impl_sdlgpu3.h>
#include <imnodes/imnodes.h>
#include <math.h>

#include "NodeManager.h"
#include "setup.h"
#include "ui.h"


int main(int, char**) {
	GraphicsContext context = createWindow();
	bool done				= false;
	int uniqueNumber		= 0;
	NodeManager nodeManager;

	while (!done) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			ImGui_ImplSDL3_ProcessEvent(&event);
			if (event.type == SDL_EVENT_QUIT || event.key.key == SDLK_ESCAPE ||
				(event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED &&
				 event.window.windowID == SDL_GetWindowID(context.window))) {
				done = true;
			}
			if (nodeManager.newNodeType != NOT_ADDING && event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
				uniqueNumber++;
				ImNodes::SetNodeScreenSpacePos(uniqueNumber, ImGui::GetMousePos());
				nodeManager.addNode(uniqueNumber, nodeManager.newNodeType);
				nodeManager.newNodeType = NOT_ADDING;
			}
		}

		if (SDL_GetWindowFlags(context.window) & SDL_WINDOW_MINIMIZED) {
			SDL_Delay(10);
			continue;
		}

		ImGui_ImplSDLGPU3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();
		ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));

		drawEditor(&context, &nodeManager, nodeManager.newNodeType != NOT_ADDING, uniqueNumber);
		drawSideBar(&nodeManager.newNodeType);

		ImGui::PopStyleVar();

		// ImGui::ShowDemoWindow();
		nodeManager.copyover();
		nodeManager.calculate();
		render(context);
	}

	destroyWindow(context);
	return 0;
}

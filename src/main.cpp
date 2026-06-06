#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl.h>
#include <imnodes/imnodes.h>
#include <math.h>

#include "new_node.h"
#include "node_manager.h"
#include "setup.h"
#include "ui.h"


int main(int, char**) {
	GraphicsContext context = createWindow();

	bool done = false;
	int c	  = 0; // to identify proper generics for combinational circuit
	int screenWidth, screenHeight;

	int unique_number = 0;
	node_manager node_man;

	node_adder isAdding = {false, NOT_ADDING};
	while (!done) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			ImGui_ImplSDL2_ProcessEvent(&event);
			if (event.type == SDL_QUIT || (event.key.keysym.sym == SDLK_ESCAPE)) {
				done = true;
			}
			if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
				event.window.windowID == SDL_GetWindowID(context.window))
				done = true;
			if (isAdding.isAdding && isAdding.new_node_typ > GENERICS) {
				c = isAdding.new_node_typ;
			} else if (isAdding.isAdding && event.type == SDL_MOUSEBUTTONDOWN) {
				unique_number++;
				ImNodes::SetNodeScreenSpacePos(unique_number, ImGui::GetMousePos());
				node_man.add_node(unique_number, "Add Node", isAdding.new_node_typ);
				isAdding = {false, NOT_ADDING};
				c		 = NOT_ADDING;
			}
		}

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(context.window);
		ImGui::NewFrame();
		ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));

		// Meat Logic goes here:
		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;

		SDL_GetWindowSize(context.window, &screenWidth, &screenHeight);
		ImVec2 windowSize{( float )screenWidth - SIDEBAR_WIDTH, ( float )screenHeight};
		ImGui::SetNextWindowSize(windowSize);
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

		node_man.render();

		ImNodes::MiniMap();
		ImNodes::EndNodeEditor();

		int link_id = -1, start_node_id = -1, start_pin_id = -1, end_node_id = -1, end_pin_id = -1;
		bool created_from_snap;
		if (ImNodes::IsLinkCreated(&start_node_id, &start_pin_id, &end_node_id, &end_pin_id, &created_from_snap)) {
			unique_number++;
			node_man.add_link(unique_number, start_node_id, start_pin_id, end_node_id, end_pin_id);
		}
		if (ImNodes::IsLinkDestroyed(&link_id)) {
			node_man.remove_link(link_id);
		}

		ImGui::End();

		drawSideBar(&isAdding, &c);

		if (isAdding.isAdding) {
			windowSize.x = ( float )screenWidth - SIDEBAR_WIDTH;
			windowSize.y = ( float )screenHeight;
			ImGui::SetNextWindowSize(windowSize);
			ImGui::SetNextWindowPos({0, 0});
			ImGui::SetNextWindowBgAlpha(0.40f);

			ImGui::Begin(
				"Adding Node",
				nullptr,
				ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
					ImGuiWindowFlags_NoTitleBar);
			ImGui::End();
		}


		ImGui::PopStyleVar();

		// ImGui::ShowDemoWindow();
		node_man.copyover();
		node_man.calculate();
		// Rendering
		ImGui::Render();

		int fb_width, fb_height;
		SDL_GL_GetDrawableSize(context.window, &fb_width, &fb_height);
		glViewport(0, 0, fb_width, fb_height);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(context.window);
	}

	destroyWindow(context);
	return 0;
}

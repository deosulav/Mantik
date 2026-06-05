#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl.h>
#include <imnodes/imnodes.h>

#include "new_node.h"
#include "node_manager.h"
#include "setup.h"

int main(int, char**) {
	GraphicsContext context = createWindow();

	bool done = false;
	int c	  = 0; // to identify proper generics for combinational circuit
	int s_w, s_h;

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

		// Meat Logic goes here:
		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;

		SDL_GetWindowSize(context.window, &s_w, &s_h);
		ImVec2 window_size{( float )s_w * 0.75f, ( float )s_h};
		ImGui::SetNextWindowSize(window_size);
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

		bool isOpen;

		window_size.x = ( float )s_w * 0.25f;
		window_size.y = ( float )s_h * 0.1f;
		ImGui::SetNextWindowSize(window_size);
		ImGui::SetNextWindowPos({( float )s_w * 0.75f, 0});

		ImGui::Begin(
			"Desc",
			&isOpen,
			ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
		ImGui::TextUnformatted("Red Color is Logic Low");
		ImGui::TextUnformatted("Green Color is Logic High");
		ImGui::End();

		window_size.x = ( float )s_w * 0.25f;
		window_size.y = ( float )s_h * 0.9f;
		ImGui::SetNextWindowSize(window_size);
		ImGui::SetNextWindowPos({( float )s_w * 0.75f, ( float )0.1 * s_h});

		ImGui::Begin(
			"Experimental",
			&isOpen,
			ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
		ImGui::TextUnformatted("Add Nodes!");


		ImGui::NewLine();
		if (ImGui::Button("  Input          "))
			isAdding = {true, INPUT};
		if (c == INPUT) {
			if (ImGui::Button("  1            "))
				isAdding = {true, INPUT_1}, c = false;
			if (ImGui::Button("  2            "))
				isAdding = {true, INPUT_2}, c = false;
			if (ImGui::Button("  3            "))
				isAdding = {true, INPUT_3}, c = false;
			if (ImGui::Button("  4            "))
				isAdding = {true, INPUT_4}, c = false;
			if (ImGui::Button("  5            "))
				isAdding = {true, INPUT_5}, c = false;
			if (ImGui::Button("  6            "))
				isAdding = {true, INPUT_6}, c = false;
			if (ImGui::Button("  7            "))
				isAdding = {true, INPUT_7}, c = false;
			if (ImGui::Button("  8            "))
				isAdding = {true, INPUT_8}, c = false;
		}

		if (ImGui::Button("  Output         "))
			isAdding = {true, OUTPUT};

		if (ImGui::Button("  And Gate       "))
			isAdding = {true, AND_GATE};
		if (c == AND_GATE) {
			if (ImGui::Button("  2 input      "))
				isAdding = {true, AND_GATE_2}, c = false;
			if (ImGui::Button("  3 input      "))
				isAdding = {true, AND_GATE_3}, c = false;
			if (ImGui::Button("  4 input      "))
				isAdding = {true, AND_GATE_4}, c = false;
			if (ImGui::Button("  5 input      "))
				isAdding = {true, AND_GATE_5}, c = false;
			if (ImGui::Button("  6 input      "))
				isAdding = {true, AND_GATE_6}, c = false;
			if (ImGui::Button("  7 input      "))
				isAdding = {true, AND_GATE_7}, c = false;
			if (ImGui::Button("  8 input      "))
				isAdding = {true, AND_GATE_8}, c = false;
		}

		if (ImGui::Button("  Or Gate        "))
			isAdding = {true, OR_GATE};
		if (c == OR_GATE) {
			if (ImGui::Button("  2 input      "))
				isAdding = {true, OR_GATE_2}, c = false;
			if (ImGui::Button("  3 input      "))
				isAdding = {true, OR_GATE_3}, c = false;
			if (ImGui::Button("  4 input      "))
				isAdding = {true, OR_GATE_4}, c = false;
			if (ImGui::Button("  5 input      "))
				isAdding = {true, OR_GATE_5}, c = false;
			if (ImGui::Button("  6 input      "))
				isAdding = {true, OR_GATE_6}, c = false;
			if (ImGui::Button("  7 input      "))
				isAdding = {true, OR_GATE_7}, c = false;
			if (ImGui::Button("  8 input      "))
				isAdding = {true, OR_GATE_8}, c = false;
		}

		if (ImGui::Button("  Not Gate       "))
			isAdding = {true, NOT_GATE};

		if (ImGui::Button("  Encoder        "))
			isAdding = {true, ENCODER};
		if (c == ENCODER) {
			if (ImGui::Button("  8:3 Encoder  "))
				isAdding = {true, ENCODER8_3}, c = false;
			if (ImGui::Button("  4:2 Encoder  "))
				isAdding = {true, ENCODER4_2}, c = false;
			if (ImGui::Button("  2:1 Encoder  "))
				isAdding = {true, ENCODER2_1}, c = false;
		}

		if (ImGui::Button("  Decoder        "))
			isAdding = {true, DECODER};
		if (c == DECODER) {
			if (ImGui::Button("  3:8 Decoder  "))
				isAdding = {true, DECODER3_8}, c = false;
			if (ImGui::Button("  2:4 Decoder  "))
				isAdding = {true, DECODER2_4}, c = false;
			if (ImGui::Button("  1:2 Decoder  "))
				isAdding = {true, DECODER1_2}, c = false;
		}

		if (ImGui::Button("  Multiplexer    "))
			isAdding = {true, MULTIPLEXER};
		if (c == MULTIPLEXER) {
			if (ImGui::Button("  8:1 Mux      "))
				isAdding = {true, MULTIPLEXER8_1}, c = false;
			if (ImGui::Button("  4:1 Mux      "))
				isAdding = {true, MULTIPLEXER4_1}, c = false;
			if (ImGui::Button("  2:1 Mux      "))
				isAdding = {true, MULTIPLEXER2_1}, c = false;
		}

		if (ImGui::Button("  DeMultiplexer  "))
			isAdding = {true, DEMULTIPLEXER};
		if (c == DEMULTIPLEXER) {
			if (ImGui::Button("  1:8 DeMUX    "))
				isAdding = {true, DEMULTIPLEXER1_8}, c = false;
			if (ImGui::Button("  1:4 DeMUX    "))
				isAdding = {true, DEMULTIPLEXER1_4}, c = false;
			if (ImGui::Button("  1:2 DeMUX    "))
				isAdding = {true, DEMULTIPLEXER1_2}, c = false;
		}
		ImGui::End();

		if (isAdding.isAdding) {
			window_size.x = ( float )s_w * 0.75f;
			window_size.y = ( float )s_h;
			ImGui::SetNextWindowSize(window_size);
			ImGui::SetNextWindowPos({0, 0});
			ImGui::SetNextWindowBgAlpha(0.40f);

			ImGui::Begin(
				"Adding Node",
				&isOpen,
				ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
					ImGuiWindowFlags_NoTitleBar);
			ImGui::End();
		}



		// ImGui::ShowDemoWindow( );
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

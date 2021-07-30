#include <GL/glew.h>

#include <SDL2/SDL.h>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>

#include <imnodes.h>

#include "node_manager.h"
#include "new_node.h"

int
main (int, char**) {
	const char* glsl_version = "#version 130";

	SDL_Window*	  window = nullptr;
	SDL_GLContext gl_context;

	if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
		printf ("Error: %s\n", SDL_GetError( ));
		return -1;
	}

	// GL 3.0 + GLSL 130
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 3);

	SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute (SDL_GL_STENCIL_SIZE, 8);
	SDL_DisplayMode current;
	SDL_GetCurrentDisplayMode (0, &current);
	window = SDL_CreateWindow (
		"imgui-node-editor example",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		1280,
		720,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	gl_context = SDL_GL_CreateContext (window);
	SDL_GL_MakeCurrent (window, gl_context);
	SDL_GL_SetSwapInterval (1); // Enable vsync

	if (glewInit( )) {
		fprintf (stderr, "Failed to initialize OpenGL loader!\n");
		return 1;
	}

	const ImVec4 clear_color = ImVec4 (0.45f, 0.55f, 0.60f, 1.00f);
	glClearColor (clear_color.x, clear_color.y, clear_color.z, clear_color.w);

	IMGUI_CHECKVERSION( );
	ImGui::CreateContext( );

	ImGui_ImplSDL2_InitForOpenGL (window, gl_context);
	ImGui_ImplOpenGL3_Init (glsl_version);

	ImNodes::CreateContext( );

	// Setup style
	ImGui::StyleColorsDark( );
	ImNodes::StyleColorsDark( );

	ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick | ImNodesAttributeFlags_EnableLinkCreationOnSnap); // WHYYYYYY YOU SOOO HIDDEN
	ImNodesEditorContext* e1 = ImNodes::EditorContextCreate( );

	bool done = false;
	int c	  = 0;
	int	 s_w, s_h;

	int unique_number = 0;
	node_manager node_man;

	node_adder isAdding = {false, NOT_ADDING};
	while (!done) {


		SDL_Event event;
		while (SDL_PollEvent (&event)) {
			ImGui_ImplSDL2_ProcessEvent (&event);
			if (event.type == SDL_QUIT)
				done = true;
			if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
				event.window.windowID == SDL_GetWindowID (window))
				done = true;
			if (isAdding.isAdding && event.type == SDL_MOUSEBUTTONDOWN && isAdding.new_node_typ>GENERICS ) {
				c = isAdding.new_node_typ;
			}
			else if (isAdding.isAdding && event.type == SDL_MOUSEBUTTONDOWN) {
				unique_number++;
				ImNodes::SetNodeScreenSpacePos(unique_number, ImGui::GetMousePos());
				node_man.add_node(unique_number, "Add Node", isAdding.new_node_typ);
				isAdding = {false, NOT_ADDING};
			}
		}

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame( );
		ImGui_ImplSDL2_NewFrame (window);
		ImGui::NewFrame( );

		// Meat Logic goes here:
		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;

		SDL_GetWindowSize (window, &s_w, &s_h);
		ImVec2 window_size{( float )s_w * 0.75f, ( float )s_h};
		ImGui::SetNextWindowSize (window_size);
		ImGui::SetNextWindowPos ({0, 0});



		ImGui::Begin (
			"Encloser",
			NULL,
			ImGuiWindowFlags_NoBringToFrontOnFocus | // Always want the Editor to be on background
				ImGuiWindowFlags_NoMove |			 // Always want the Editor to Stay on top right
				ImGuiWindowFlags_NoResize |			 // Always want it take all space (on background)
				ImGuiWindowFlags_NoTitleBar			 // Always want the title bar to never appear
		);
		ImNodes::EditorContextSet (e1);
		ImNodes::BeginNodeEditor( );


		node_man.render();

		ImNodes::MiniMap();
		ImNodes::EndNodeEditor( );

		int link_id = -1, start_node_id = -1, start_pin_id = -1, end_node_id = -1, end_pin_id = -1;
		bool created_from_snap;
		if (ImNodes::IsLinkCreated(&start_node_id, &start_pin_id, &end_node_id, &end_pin_id, &created_from_snap)) {
			unique_number++;
			node_man.add_link(unique_number, start_node_id, start_pin_id, end_node_id, end_pin_id);
		}
		if (ImNodes::IsLinkDestroyed(&link_id)) {
			node_man.remove_link(link_id);
		}

		ImGui::End( );

		bool isOpen;

		window_size.x = ( float )s_w * 0.25f;
		window_size.y = ( float )s_h;
		ImGui::SetNextWindowSize (window_size);
		ImGui::SetNextWindowPos ({( float )s_w * 0.75f, 0});

		ImGui::Begin("Experimental", &isOpen, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar );
		ImGui::TextUnformatted("Add Nodes!");


		ImGui::NewLine();
		if (ImGui::Button ("Input"))
			isAdding = {true, INPUT};
		if (c == INPUT) {
			if (ImGui::Button ("  1  "))
				isAdding = {true, INPUT_1}, c = false;
			if (ImGui::Button ("  2  "))
				isAdding = {true, INPUT_2}, c = false;
			if (ImGui::Button ("  3  "))
				isAdding = {true, INPUT_3}, c = false;
			if (ImGui::Button ("  4  "))
				isAdding = {true, INPUT_4}, c = false;
			if (ImGui::Button ("  5  "))
				isAdding = {true, INPUT_5}, c = false;
			if (ImGui::Button ("  6  "))
				isAdding = {true, INPUT_6}, c = false;
			if (ImGui::Button ("  7  "))
				isAdding = {true, INPUT_7}, c = false;
			if (ImGui::Button ("  8  "))
				isAdding = {true, INPUT_8}, c = false;
		}
		if (ImGui::Button("And Gate"))
			isAdding = {true, AND_GATE};
		if (ImGui::Button("Or Gate"))
			isAdding = {true, OR_GATE};
		if (ImGui::Button("Not Gate"))
			isAdding = {true, NOT_GATE};
		if (ImGui::Button ("Multiplexer"))
			isAdding = {true, MULTIPLEXER};
		if (c==MULTIPLEXER) {
			if (ImGui::Button ("8:1"))
				isAdding = {true, MULTIPLEXER8_1},c = false;
			if (ImGui::Button ("4:1"))
				isAdding = {true, MULTIPLEXER4_1}, c = false;
			if (ImGui::Button ("2:1"))
				isAdding = {true, MULTIPLEXER2_1}, c = false;
		}
		if (ImGui::Button ("DeMultiplexer"))
			isAdding = {true, DEMULTIPLEXER};
		if (c == DEMULTIPLEXER) {
			if (ImGui::Button ("8:1"))
				isAdding = {true, DEMULTIPLEXER1_8}, c = false;
			if (ImGui::Button ("4:1"))
				isAdding = {true, DEMULTIPLEXER1_4}, c = false;
			if (ImGui::Button ("2:1"))
				isAdding = {true, DEMULTIPLEXER1_2}, c = false;
		}
		ImGui::End();

		if(isAdding.isAdding){
			window_size.x = ( float )s_w * 0.75f;
			window_size.y = ( float )s_h;
			ImGui::SetNextWindowSize (window_size);
			ImGui::SetNextWindowPos ({0, 0});
			ImGui::SetNextWindowBgAlpha(0.40f);

			ImGui::Begin("Adding Node", &isOpen, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar );
			ImGui::End();
		}


		//ImGui::ShowDemoWindow( );

		node_man.copyover ( );
		node_man.calculate ( );
		// Rendering
		ImGui::Render( );

		int fb_width, fb_height;
		SDL_GL_GetDrawableSize (window, &fb_width, &fb_height);
		glViewport (0, 0, fb_width, fb_height);
		glClear (GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_RenderDrawData (ImGui::GetDrawData( ));
		SDL_GL_SwapWindow (window);
	}

	ImNodes::EditorContextFree (e1);

	ImGui_ImplOpenGL3_Shutdown( );
	ImGui_ImplSDL2_Shutdown( );
	ImGui::DestroyContext( );

	SDL_GL_DeleteContext (gl_context);
	SDL_DestroyWindow (window);
	SDL_Quit( );

	return 0;
}

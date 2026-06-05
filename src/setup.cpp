
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl.h>
#include <stdio.h>

#include "setup.h"

GraphicsContext createWindow() {
	const char* glsl_version = "#version 130";

	SDL_Window* window = nullptr;
	SDL_GLContext gl_context;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
		printf("Error: %s\n", SDL_GetError());
		return {};
	}

	// GL 3.0 + GLSL 130
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	SDL_DisplayMode current;
	SDL_GetCurrentDisplayMode(0, &current);
	window = SDL_CreateWindow(
		"Mantik",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		1280,
		720,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	gl_context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, gl_context);
	SDL_GL_SetSwapInterval(1); // Enable vsync

	if (glewInit()) {
		fprintf(stderr, "Failed to initialize OpenGL loader!\n");
		return {};
	}

	const ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImNodes::CreateContext();
	// Setup style
	ImGui::StyleColorsDark();
	ImNodes::StyleColorsDark();

	ImNodes::PushAttributeFlag(
		ImNodesAttributeFlags_EnableLinkDetachWithDragClick |
		ImNodesAttributeFlags_EnableLinkCreationOnSnap); // WHYYYYYY YOU SOOO HIDDEN
	ImNodesEditorContext* editorContext = ImNodes::EditorContextCreate();
	return GraphicsContext{window, gl_context, editorContext};
}

void destroyWindow(GraphicsContext context) {
	ImNodes::EditorContextFree(context.editorContext);

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(context.glContext);
	SDL_DestroyWindow(context.window);
	SDL_Quit();
}

#include <SDL3/SDL.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl3.h>
#include <imgui/imgui_impl_sdlgpu3.h>
#include <stdio.h>

#include "setup.h"

GraphicsContext createWindow() {

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		printf("SDL Init Error: %s\n", SDL_GetError());
		return {};
	}

	float main_scale			 = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
	SDL_WindowFlags window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;
	SDL_Window* window =
		SDL_CreateWindow("Mantik", ( int )(1280 * main_scale), ( int )(720 * main_scale), window_flags);
	if (window == nullptr) {
		printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
		return {};
	}
	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	SDL_ShowWindow(window);

	SDL_GPUDevice* gpu_device = SDL_CreateGPUDevice(
		SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL |
			SDL_GPU_SHADERFORMAT_METALLIB,
		true,
		nullptr);
	if (gpu_device == nullptr) {
		printf("Error: SDL_CreateGPUDevice(): %s\n", SDL_GetError());
		return {};
	}

	if (!SDL_ClaimWindowForGPUDevice(gpu_device, window)) {
		printf("Error: SDL_ClaimWindowForGPUDevice(): %s\n", SDL_GetError());
		return {};
	}
	SDL_SetGPUSwapchainParameters(gpu_device, window, SDL_GPU_SWAPCHAINCOMPOSITION_SDR, SDL_GPU_PRESENTMODE_VSYNC);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImNodes::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	( void )io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	// Setup style
	ImGui::StyleColorsDark();
	ImNodes::StyleColorsDark();

	// Setup scaling
	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(main_scale); // Bake a fixed style scale. (until we have a solution for dynamic style scaling,
									 // changing this requires resetting Style + calling this again)
	style.FontScaleDpi = main_scale; // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true
									 // automatically overrides this for every window depending on the current monitor)

	ImGui_ImplSDL3_InitForSDLGPU(window);
	ImGui_ImplSDLGPU3_InitInfo init_info = {};
	init_info.Device					 = gpu_device;
	init_info.ColorTargetFormat			 = SDL_GetGPUSwapchainTextureFormat(gpu_device, window);
	init_info.MSAASamples				 = SDL_GPU_SAMPLECOUNT_1;			 // Only used in multi-viewports mode.
	init_info.SwapchainComposition		 = SDL_GPU_SWAPCHAINCOMPOSITION_SDR; // Only used in multi-viewports mode.
	init_info.PresentMode				 = SDL_GPU_PRESENTMODE_VSYNC;
	ImGui_ImplSDLGPU3_Init(&init_info);


	ImNodes::PushAttributeFlag(
		ImNodesAttributeFlags_EnableLinkDetachWithDragClick |
		ImNodesAttributeFlags_EnableLinkCreationOnSnap); // WHYYYYYY YOU SOOO HIDDEN
	ImNodesEditorContext* editorContext = ImNodes::EditorContextCreate();
	return GraphicsContext{window, gpu_device, editorContext};
}

void destroyWindow(GraphicsContext context) {
	ImNodes::EditorContextFree(context.editorContext);


	SDL_WaitForGPUIdle(context.gpuDevice);
	ImGui_ImplSDL3_Shutdown();
	ImGui_ImplSDLGPU3_Shutdown();
	ImGui::DestroyContext();

	SDL_ReleaseWindowFromGPUDevice(context.gpuDevice, context.window);
	SDL_DestroyGPUDevice(context.gpuDevice);
	SDL_DestroyWindow(context.window);
	SDL_Quit();
}

ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
void render(GraphicsContext context) {
	ImGui::Render();

	int fb_width, fb_height;
	ImDrawData* draw_data	= ImGui::GetDrawData();
	const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);

	SDL_GPUCommandBuffer* command_buffer = SDL_AcquireGPUCommandBuffer(context.gpuDevice);

	SDL_GPUTexture* swapchain_texture;
	SDL_WaitAndAcquireGPUSwapchainTexture(command_buffer, context.window, &swapchain_texture, nullptr, nullptr);

	if (swapchain_texture != nullptr && !is_minimized) {
		ImGui_ImplSDLGPU3_PrepareDrawData(draw_data, command_buffer);

		SDL_GPUColorTargetInfo target_info = {};
		target_info.texture				   = swapchain_texture;
		target_info.clear_color			   = SDL_FColor{clear_color.x, clear_color.y, clear_color.z, clear_color.w};
		target_info.load_op				   = SDL_GPU_LOADOP_CLEAR;
		target_info.store_op			   = SDL_GPU_STOREOP_STORE;
		target_info.mip_level			   = 0;
		target_info.layer_or_depth_plane   = 0;
		target_info.cycle				   = false;
		SDL_GPURenderPass* render_pass	   = SDL_BeginGPURenderPass(command_buffer, &target_info, 1, nullptr);

		ImGui_ImplSDLGPU3_RenderDrawData(draw_data, command_buffer, render_pass);
		SDL_EndGPURenderPass(render_pass);
	}

	SDL_SubmitGPUCommandBuffer(command_buffer);
}

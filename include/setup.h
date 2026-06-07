#ifndef SETUP_H_
#define SETUP_H_

#include "SDL3/SDL.h"
#include <imnodes/imnodes.h>

struct GraphicsContext {
	SDL_Window* window;
	SDL_GPUDevice* gpuDevice;
	ImNodesEditorContext* editorContext;
};

GraphicsContext createWindow();
void destroyWindow(GraphicsContext context);
void render(GraphicsContext context);
#endif
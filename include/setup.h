#ifndef SETUP_H_
#define SETUP_H_

#include "SDL2/SDL.h"
#include <imnodes/imnodes.h>

struct GraphicsContext {
	SDL_Window* window;
	SDL_GLContext glContext;
	ImNodesEditorContext* editorContext;
};

GraphicsContext createWindow();
void destroyWindow(GraphicsContext context);
#endif
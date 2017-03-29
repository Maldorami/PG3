#ifndef ENGINE_H
#define ENGINE_H

#include "Window.h"
#include "Renderer.h"
#include "Game.h"
#include "pg1_directinput.h"
#include "pg1_timer.h"

#define DLLexport __declspec(dllexport)

class Engine{
private:
	unsigned int Width;
	unsigned int Height;
	unsigned int nShow;
	HINSTANCE instance;
	Renderer* render;
	Game* gameV;
	
	DirectInput* input;
	Timer* timer;

public:
	Window* window;
	DLLexport Engine(HINSTANCE hInstance, unsigned int uiWidth, unsigned int uiHeight, int nCmdShow);
	DLLexport ~Engine();
	DLLexport bool init();
	DLLexport void run();
	DLLexport void setGame(Game* game);
};

#endif
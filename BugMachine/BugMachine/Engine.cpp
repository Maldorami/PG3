#include "Engine.h"
#include <sstream>
//---------------------------------------------------------------------------
Engine::Engine(HINSTANCE hInstance, unsigned int uiWidth, unsigned int uiHeight, int nCmdShow)
	:
	instance(hInstance),
	Width(uiWidth),
	Height(uiHeight),
	nShow(nCmdShow),
	window(new Window()),
	render(new Renderer()),
	input(new DirectInput()),
	timer(new Timer()),
	gameV(NULL)

	{
	}
//---------------------------------------------------------------------------
void Engine::setGame(Game* game){
	gameV = game;
	gameV->setDone(false);
}
//---------------------------------------------------------------------------
bool Engine::init(){ 
	if(!window->create(instance, Width, Height, nShow))
		return false;

	if (!render->init(window->handler)){
		return false;
	}
	if (!gameV){
		return false;
	}
	if (!gameV->init(*render))
	{
		return false;
	}
	if (!input->init(instance,window->handler))
	{
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------
void Engine::run(){

	MSG Msg;
	timer->firstMeasure();

	// Step 3: The Message Loop 
	while (!gameV->isDone())
	{
		timer->measure();

		static std::stringstream showFPS;
		showFPS.str("");
		showFPS << "BugMachine - " << timer->fps() << " FPS";
		window->setTitle(showFPS.str());

		input->reacquire();
		render->beginFrame();
		gameV->frame(*render, *input, *timer);
		render->endFrame();

		if(PeekMessage(&Msg, NULL, 0, 0,PM_REMOVE))
		{
			switch (Msg.message)
			{
				case WM_QUIT:
				{
					gameV->setDone(true);
					break;
				}
			}

			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
	}

	gameV->deinit();
}
//---------------------------------------------------------------------------
Engine::~Engine(){
	delete render;
	delete window;
	delete input;
	delete timer;

	render = NULL;
	window = NULL;
	input = NULL;
	timer = NULL;
}
//---------------------------------------------------------------------------
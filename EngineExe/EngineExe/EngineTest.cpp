//MALDONADO, RAMIRO
//PUENTE, ALEJANDRO
//PROGRAMACION DE GRAFICOS 1 - MOTOR GRAFICO 2D

#include "Engine.h"
#include "Pacman.h"

#pragma warning(disable : 4996)

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	/*AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);*/

	Engine engine(hInstance, 1280, 720, nCmdShow);

	Pacman* pkGame = new Pacman();
	engine.setGame(pkGame);

	if (!engine.init()) return 1;

	engine.run();

	delete pkGame;
	pkGame = NULL;

	return 0;
}
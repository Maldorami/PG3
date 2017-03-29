#ifndef PACMAN_H
#define PACMAN_H
#include "Game.h"

class Pacman : public Game{
public:
	bool init(Renderer& rendi);
	void frame(Renderer& rkRenderer,Input& input, Timer& timer);
	void deinit(){};

private:
	Camera* cam;
	Node* nodo1;
	Frustum* frustum;
	Importador* importador;
	Mesh* teapot;

	Node* cube;
	Node* cube2;
	Node* cube3;
	Node* cube4;
	Node* cube5;
	Node* cube6;
	Node* cube7;
	Node* cube8;

	Text _text;
};
#endif
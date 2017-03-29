#ifndef IMPORTADOR_H
#define IMPORTADOR_H
#include "Node.h"
#include "Mesh.h"
#include <vector>

#define DLLexport __declspec(dllexport)
class Importador{
public:
	DLLexport Importador(Renderer& rkRenderer);
	DLLexport bool importScene(std::string rkFilename, Node& orkSceneRoot);
	
	Renderer rendi;
};
#endif IMPORTADOR_H

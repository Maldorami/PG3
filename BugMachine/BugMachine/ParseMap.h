#ifndef PARSEMAP_H
#define PARSEMAP_H
#define DLLexport __declspec(dllexport)
#include "Renderer.h"
#include "Map.h"
#include "Sprite.h"
class ParseMap{
public:
	DLLexport ParseMap();
	DLLexport ~ParseMap();
	// constructor
public:
	DLLexport bool importMap(Map& rkMap, Renderer& Renderer, const std::string& Filename);
};
#endif
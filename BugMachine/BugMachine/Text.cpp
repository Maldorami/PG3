#include "Text.h"
//---------------------------------------------------------------------------
#include <d3d9.h>
#pragma comment (lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment (lib, "d3dx9.lib")
//---------------------------------------------------------------------------
Text::Text()
:
_Font(NULL),
_x(0),
_y(0),
_width(0),
_height(0),
_charSize(0){}
//---------------------------------------------------------------------------
void Text::createText(int x, int y, int width, int height, int charSize, std::string textFont, std::string text, Renderer& renderer){

	_text = text;

	SetRect(&_rect, x, y, width, height);

	D3DXCreateFont(renderer.d3ddev,	charSize, 0, FW_NORMAL,	1, 0, DEFAULT_CHARSET,	OUT_DEFAULT_PRECIS,
				   ANTIALIASED_QUALITY, FF_DONTCARE, textFont.c_str(), &_Font);
}
//---------------------------------------------------------------------------
void Text::setText (std::string text){
	_text = text;
}
//---------------------------------------------------------------------------
void Text::draw(Renderer& renderer){
	renderer.drawText(_Font, _text, _rect);
}

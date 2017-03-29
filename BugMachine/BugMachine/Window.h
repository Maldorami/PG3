#ifndef WINDOW_H
#define WINDOW_H

#include <Windows.h>
#include <string>

class Window
{
public:
	HWND handler;
	bool create(HINSTANCE hInstance, unsigned int uiWidth, unsigned int uiHeight,int nCmdShow);
	HWND hwnd() const;
	void setTitle(const std::string& title);
};
#endif
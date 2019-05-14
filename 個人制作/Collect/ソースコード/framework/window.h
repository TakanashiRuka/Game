#pragma once

#include <Windows.h>

constexpr unsigned int init_width = 1280U;
constexpr unsigned int init_height = 720U;

class Window
{
private:
	HWND hwnd_;

	static unsigned int width_;
	static unsigned int height_;

public:
	Window(const char* name);

public:
	HWND const GetHwnd();

	template<class T = unsigned int>
	static T const GetWidth();
	template<class T = unsigned int>
	static T const GetHeight();

private:
	static LRESULT __stdcall WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
};

template<class T>
T const Window::GetWidth()
{
	return static_cast<T>(Window::width_);
}

template<class T>
T const Window::GetHeight()
{
	return static_cast<T>(Window::height_);
}
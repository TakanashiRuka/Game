#pragma once

#include <Windows.h>

#include <string>

template<class T>
inline void SafeDelete(T*& t)
{
	if (t)
	{
		delete t;
		t = nullptr;
	}
}

template<class T>
inline void SafeRelease(T*& t)
{
	if (t)
	{
		t->Release();
		t = nullptr;
	}
}

inline void ErrorMessage(std::string text)
{
	MessageBox(nullptr, text.c_str(), nullptr, MB_ICONERROR | MB_OK);
}
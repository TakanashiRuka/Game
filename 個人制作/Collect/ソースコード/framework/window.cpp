#include "..\framework\window.h"

#include "..\ImGui\imgui.h"
#include "..\ImGui\imgui_impl_dx11.h"

#pragma comment(lib,"winmm.lib")

unsigned int Window::width_ = 0;
unsigned int Window::height_ = 0;

Window::Window(const char * name)
{
	HINSTANCE hinstance = GetModuleHandle(nullptr);
	WNDCLASSEX wc;
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = Window::WndProc;
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hInstance = hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "Collect";
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&wc);

	DWORD style;
	unsigned int x;
	unsigned int y;
	RECT desktop;
	GetWindowRect(GetDesktopWindow(), &desktop);
	if ((desktop.right - init_width) > 0 &&
		(desktop.bottom - init_height) > 0)
	{
		style = WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME;

		RECT frame = { 0, 0, static_cast<long>(init_width), static_cast<long>(init_height) };
		AdjustWindowRectEx(&frame, style, false, NULL);

		Window::width_ = static_cast<unsigned int>(frame.right - frame.left);
		Window::height_ = static_cast<unsigned int>(frame.bottom - frame.top);
		x = (static_cast<unsigned int>(desktop.right) - Window::width_) / 2;
		y = (static_cast<unsigned int>(desktop.bottom) - Window::height_) / 2;
	}
	else
	{
		style = WS_POPUP;

		x = static_cast<unsigned int>(desktop.left);
		y = static_cast<unsigned int>(desktop.top);
		Window::width_ = static_cast<unsigned int>(desktop.right);
		Window::height_ = static_cast<unsigned int>(desktop.bottom);
	}

	this->hwnd_ = CreateWindowEx(NULL, wc.lpszClassName, name, style, x, y, Window::width_, Window::height_, NULL, NULL, hinstance, NULL);
	ShowWindow(this->hwnd_, SW_SHOW);
	UpdateWindow(this->hwnd_);
}

HWND const Window::GetHwnd()
{
	return this->hwnd_;
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT Window::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);

	switch (msg)
	{
	case WM_KEYDOWN:
	{
		switch (wparam)
		{
		case VK_ESCAPE:
		{
			if (IDYES == MessageBox(hwnd, "終了しますか？", "終了メッセージ", MB_YESNO | MB_DEFBUTTON2))
				DestroyWindow(hwnd);
		}
		break;
		}
	}
	break;

	case WM_CLOSE:
	{
		if (IDYES == MessageBox(hwnd, "終了しますか？", "終了メッセージ", MB_YESNO | MB_DEFBUTTON2))
			DestroyWindow(hwnd);
		else
			return 0;
	}
	break;

	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	break;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}
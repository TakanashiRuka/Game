#pragma once

#include <Windows.h>
#include <DirectXMath.h>

#include <string>

class Debug
{
public:
	static void Init(HWND hwnd);
	static void Uninit();
	static void Begin();
	static void End();

public:
	static void DrawLight(DirectX::XMFLOAT4& light, DirectX::XMFLOAT4& ambient, DirectX::XMFLOAT4& diffuse, DirectX::XMFLOAT4& specular);
	static void DrawCameraTPS(DirectX::XMFLOAT3& eye, DirectX::XMFLOAT3& at, DirectX::XMFLOAT3& up, DirectX::XMFLOAT3& pos, DirectX::XMFLOAT2& rot, bool& free);
	static void DrawObject(std::string object, DirectX::XMFLOAT3& pos);
	static void DrawDepth(void* shader_resource_view, float width, float height);
	static void DrawInput();
	static void DrawWindow(float* color);
};
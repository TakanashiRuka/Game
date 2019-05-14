#pragma once

#include "..\framework\rendering_pass.h"

#include <Windows.h>
#include <d3d11.h>

constexpr unsigned int depth_texture_size = 1;

enum class RasterizerState : unsigned int
{
	Default,
	CullNone,
	Wireframe,
	Max,
};

enum class BlendState : unsigned int
{
	Default,
	AlphaToCoverage,
	BlendEnable,
	Max,
};

class Graphics
{
	friend class Game;

private:
	enum Format
	{
		Depth,
		DepthCascade1,
		Base,
		Max,
	};

public:
	ID3D11Device* device_ = nullptr;
	ID3D11DeviceContext* device_context_ = nullptr;
	IDXGISwapChain* swap_chain_ = nullptr;

private:
	ID3D11RenderTargetView* render_target_view_[Format::Max];
	ID3D11DepthStencilView* depth_stencil_view_[Format::Max];
	ID3D11ShaderResourceView* shader_resource_view_[Format::Max];
	D3D11_VIEWPORT viewport_[Format::Max];
	ID3D11RasterizerState* rs_[static_cast<unsigned int>(RasterizerState::Max)];
	ID3D11BlendState* blend_state_[static_cast<unsigned int>(BlendState::Max)];

	float clear_color_[4] = { 0, 0, 1, 1 };

public:
	Graphics(unsigned int width, unsigned int height, unsigned int fps, HWND hwnd);
	~Graphics();

public:
	void SetRasterizerState(RasterizerState rasterizer_state);
	void SetBlendState(BlendState render_state);

private:
	void Begin(RenderingPass pass);
	void DebugDraw();
	void End();
};
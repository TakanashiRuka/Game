#include "..\graphics\directx11.h"
#include "..\framework\utility.h"

#include "..\framework\debug.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")

Graphics::Graphics(unsigned int width, unsigned int height, unsigned int fps, HWND hwnd)
{
	D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &this->device_, nullptr, &this->device_context_);

	DXGI_SAMPLE_DESC sd = {};
	for (unsigned int cnt = 0; cnt < D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; ++cnt)
	{
		UINT quality;
		if (SUCCEEDED(this->device_->CheckMultisampleQualityLevels(DXGI_FORMAT_D24_UNORM_S8_UINT, cnt, &quality)))
		{
			if (0 < quality)
			{
				sd.Count = cnt;
				sd.Quality = quality - 1;
			}
		}
	}
	if (sd.Count > 4)
	{
		sd.Count = 4;
	}
#ifdef _DEBUG
	sd.Count = 1;
#endif // _DEBUG
	sd.Quality = 0;

	DXGI_SWAP_CHAIN_DESC scd = {};
	scd.BufferDesc.Width = width;
	scd.BufferDesc.Height = height;
	scd.BufferDesc.RefreshRate.Numerator = fps;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.SampleDesc = sd;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 1;
	scd.OutputWindow = hwnd;
	scd.Windowed = true;
#ifdef _DEBUG
	scd.Flags = D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

	D3D_FEATURE_LEVEL feature_levels = D3D_FEATURE_LEVEL_11_0;
	D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, &feature_levels, 1, D3D11_SDK_VERSION, &scd, &this->swap_chain_, &this->device_, nullptr, &this->device_context_);

	ID3D11Texture2D* back_buffer_tex2d_rtv;
	this->swap_chain_->GetBuffer(NULL, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer_tex2d_rtv);

	this->device_->CreateRenderTargetView(back_buffer_tex2d_rtv, nullptr, &this->render_target_view_[Format::Base]);

	D3D11_TEXTURE2D_DESC tex2d_desc = {};
	tex2d_desc.Width = width;
	tex2d_desc.Height = height;
	tex2d_desc.MipLevels = 1;
	tex2d_desc.ArraySize = 1;
	tex2d_desc.SampleDesc = sd;
	tex2d_desc.Usage = D3D11_USAGE_DEFAULT;
	tex2d_desc.CPUAccessFlags = 0;
	tex2d_desc.MiscFlags = 0;
	tex2d_desc.Format = DXGI_FORMAT_D32_FLOAT;
	tex2d_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	ID3D11Texture2D* texture;
	this->device_->CreateTexture2D(&tex2d_desc, nullptr, &texture);

	ID3D11Texture2D* back_buffer_tex2d_dsv;
	this->device_->CreateTexture2D(&tex2d_desc, nullptr, &back_buffer_tex2d_dsv);
	this->device_->CreateDepthStencilView(back_buffer_tex2d_dsv, nullptr, &this->depth_stencil_view_[Format::Base]);

	//シャドウマップ
	tex2d_desc.Width = 1024 * depth_texture_size;
	tex2d_desc.Height = 1024 * depth_texture_size;
	tex2d_desc.MipLevels = 1;
	tex2d_desc.ArraySize = 1;
	tex2d_desc.Format = DXGI_FORMAT_R32_FLOAT;
	tex2d_desc.SampleDesc.Count = 1;
	tex2d_desc.SampleDesc.Quality = 0;
	tex2d_desc.Usage = D3D11_USAGE_DEFAULT;
	tex2d_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	tex2d_desc.CPUAccessFlags = 0;
	tex2d_desc.MiscFlags = 0;

	ID3D11Texture2D* depth_tex2d;
	this->device_->CreateTexture2D(&tex2d_desc, nullptr, &depth_tex2d);

	D3D11_RENDER_TARGET_VIEW_DESC rtvd;
	rtvd.Format = tex2d_desc.Format;
	rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvd.Texture2D.MipSlice = 0;

	this->device_->CreateRenderTargetView(depth_tex2d, &rtvd, &this->render_target_view_[Format::Depth]);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
	srvd.Format = rtvd.Format;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MipLevels = 1;

	this->device_->CreateShaderResourceView(depth_tex2d, &srvd, &this->shader_resource_view_[Format::Depth]);

	tex2d_desc.Width = 1024 * depth_texture_size;
	tex2d_desc.Height = 1024 * depth_texture_size;
	tex2d_desc.MipLevels = 1;
	tex2d_desc.ArraySize = 1;
	tex2d_desc.Format = DXGI_FORMAT_D32_FLOAT;
	tex2d_desc.SampleDesc.Count = 1;
	tex2d_desc.SampleDesc.Quality = 0;
	tex2d_desc.Usage = D3D11_USAGE_DEFAULT;
	tex2d_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tex2d_desc.CPUAccessFlags = 0;
	tex2d_desc.MiscFlags = 0;

	this->device_->CreateTexture2D(&tex2d_desc, nullptr, &depth_tex2d);

	this->device_->CreateDepthStencilView(depth_tex2d, nullptr, &this->depth_stencil_view_[Format::Depth]);

	//VertexShaderのみ
	//tex2d_desc.Width = 1024 * depth_texture_size;
	//tex2d_desc.Height = 1024 * depth_texture_size;
	//tex2d_desc.MipLevels = 1;
	//tex2d_desc.ArraySize = 1;
	//tex2d_desc.Format = DXGI_FORMAT_R16_TYPELESS;
	//tex2d_desc.SampleDesc.Count = 1;
	//tex2d_desc.SampleDesc.Quality = 0;
	//tex2d_desc.Usage = D3D11_USAGE_DEFAULT;
	//tex2d_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	//tex2d_desc.CPUAccessFlags = 0;
	//tex2d_desc.MiscFlags = 0;

	//ID3D11Texture2D* depth_tex2d;
	//this->device_->CreateTexture2D(&tex2d_desc, nullptr, &depth_tex2d);

	//D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
	//srvd.Format = DXGI_FORMAT_R16_UNORM;
	//srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	//srvd.Texture2D.MipLevels = 1;
	//srvd.Texture2D.MostDetailedMip = 0;

	//this->device_->CreateShaderResourceView(depth_tex2d, &srvd, &this->shader_resource_view_);

	//D3D11_DEPTH_STENCIL_VIEW_DESC dsvd = {};
	//dsvd.Format = DXGI_FORMAT_D16_UNORM;
	//dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	//dsvd.Texture2D.MipSlice = 0;

	//this->device_->CreateDepthStencilView(depth_tex2d, &dsvd, &this->depth_stencil_view_[Format::Depth]);

	this->viewport_[Format::Base].Width = static_cast<float>(width);
	this->viewport_[Format::Base].Height = static_cast<float>(height);
	this->viewport_[Format::Base].MinDepth = 0;
	this->viewport_[Format::Base].MaxDepth = 1;
	this->viewport_[Format::Base].TopLeftX = 0;
	this->viewport_[Format::Base].TopLeftY = 0;

	this->viewport_[Format::Depth] = this->viewport_[Format::Base];
	this->viewport_[Format::Depth].Width = static_cast<float>(1024) * depth_texture_size;
	this->viewport_[Format::Depth].Height = static_cast<float>(1024) * depth_texture_size;

	D3D11_RASTERIZER_DESC rd = {};
	rd.CullMode = D3D11_CULL_BACK;
	rd.FillMode = D3D11_FILL_SOLID;
	rd.DepthClipEnable = false;
	this->device_->CreateRasterizerState(&rd, &this->rs_[static_cast<unsigned int>(RasterizerState::Default)]);
	rd.CullMode = D3D11_CULL_NONE;
	this->device_->CreateRasterizerState(&rd, &this->rs_[static_cast<unsigned int>(RasterizerState::CullNone)]);
	rd.CullMode = D3D11_CULL_BACK;
	rd.FillMode = D3D11_FILL_WIREFRAME;
	this->device_->CreateRasterizerState(&rd, &this->rs_[static_cast<unsigned int>(RasterizerState::Wireframe)]);
	this->device_context_->RSSetState(this->rs_[static_cast<unsigned int>(RasterizerState::Default)]);

	D3D11_RENDER_TARGET_BLEND_DESC rtbd = {};
	rtbd.BlendEnable = false;
	rtbd.SrcBlend = rtbd.SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	rtbd.DestBlend = rtbd.DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	rtbd.BlendOp = rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	D3D11_BLEND_DESC bd = {};
	bd.AlphaToCoverageEnable = true;
	bd.IndependentBlendEnable = false;
	bd.RenderTarget[0] = rtbd;
	this->device_->CreateBlendState(&bd, &this->blend_state_[static_cast<unsigned int>(BlendState::AlphaToCoverage)]);
	bd.AlphaToCoverageEnable = false;
	bd.RenderTarget[0].BlendEnable = true;
	this->device_->CreateBlendState(&bd, &this->blend_state_[static_cast<unsigned int>(BlendState::BlendEnable)]);
}

Graphics::~Graphics()
{	
	SafeRelease(this->swap_chain_);
	SafeRelease(this->device_context_);
	SafeRelease(this->render_target_view_[Format::Depth]);
	SafeRelease(this->render_target_view_[Format::Base]);
	SafeRelease(this->depth_stencil_view_[Format::Depth]);
	SafeRelease(this->depth_stencil_view_[Format::Base]);
	SafeRelease(this->device_);
}

void Graphics::SetRasterizerState(RasterizerState rasterizer_state)
{
	this->device_context_->RSSetState(this->rs_[static_cast<unsigned int>(rasterizer_state)]);
}

void Graphics::SetBlendState(BlendState blend_state)
{
	(blend_state == BlendState::Default)
		? this->device_context_->OMSetBlendState(nullptr, 0, 0xffffffff)
		: this->device_context_->OMSetBlendState(this->blend_state_[static_cast<unsigned int>(blend_state)], 0, 0xffffffff);
}

void Graphics::Begin(RenderingPass pass)
{
	switch (pass)
	{
	case RenderingPass::Depth:
	{
		this->device_context_->RSSetViewports(1, &this->viewport_[Format::Depth]);
		this->device_context_->OMSetRenderTargets(1, &this->render_target_view_[Format::Depth], this->depth_stencil_view_[Format::Depth]);

		this->device_context_->ClearRenderTargetView(this->render_target_view_[Format::Depth], this->clear_color_);
		this->device_context_->ClearDepthStencilView(this->depth_stencil_view_[Format::Depth], D3D11_CLEAR_DEPTH, 1, 0);

		this->device_context_->PSSetShader(nullptr, nullptr, 0);
	}
	break;

	case RenderingPass::Base:
	{
		this->device_context_->RSSetViewports(1, &this->viewport_[Format::Base]);
		this->device_context_->OMSetRenderTargets(1, &this->render_target_view_[Format::Base], this->depth_stencil_view_[Format::Base]);

		this->device_context_->ClearRenderTargetView(this->render_target_view_[Format::Base], this->clear_color_);
		this->device_context_->ClearDepthStencilView(this->depth_stencil_view_[Format::Base], D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

		this->device_context_->PSSetShaderResources(4, 1, &this->shader_resource_view_[Format::Depth]);
	}
	break;

	case RenderingPass::UnableDepthStencil:
	{
		this->device_context_->OMSetRenderTargets(1, &this->render_target_view_[Format::Base], nullptr);
	}
	break;

	case RenderingPass::Debug:
	{
		Debug::Begin();
	}
	break;
	}
}

void Graphics::DebugDraw()
{
	Debug::DrawWindow(this->clear_color_);
	Debug::DrawDepth(this->shader_resource_view_[Format::Depth], 256, 256);
}

void Graphics::End()
{
	Debug::End();

	this->swap_chain_->Present(0, 0);
}
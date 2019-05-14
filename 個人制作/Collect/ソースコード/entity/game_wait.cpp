#include "..\entity\game_wait.h"

#include "..\graphics\directx11.h"
#include "..\framework\data.h"
#include "..\model\model_import.h"
#include "..\component\draw\draw_indexed.h"
#include "..\framework\window.h"
#include "..\input\keyboard.h"
#include "..\input\x_input.h"

constexpr char* hlsl = "ui";
constexpr InputLayoutType input_layout = InputLayoutType::Simple;
constexpr SamplerStateType sampler_state = SamplerStateType::Simple;
constexpr Texture texture_keyboard = Texture::T_Game_wait_;
constexpr Texture texture_xinput = Texture::T_Game_wait;

GameWait::GameWait(Scene* parent) : Entity(parent)
{
	this->tag_ = "game wait";

	auto model = ModelImport::UIData(Window::GetWidth<float>() / 2 - 717.f / 2, Window::GetHeight<float>() - 147, 717, 47, 0, 0, 1, 1);

	this->data_->CreateVertexShaderAndInputLayout(hlsl, input_layout);
	this->data_->CreatePixelShader(hlsl);
	this->data_->CreateVertexBuffers(this->tag_, VertexBufferType::UI, model);
	this->data_->CreateIndexBuffers(this->tag_, model);
	this->data_->CreateSamplerState(sampler_state);
	this->data_->CreateTexture(texture_keyboard);
	this->data_->CreateTexture(texture_xinput);

	this->AddComponent<DrawIndexed>(model.meshes.size());
}

GameWait::~GameWait()
{

}

void GameWait::Update()
{
	if (KeyBoard::Trigger(VK_RETURN) || XInput::Trigger(XINPUT_GAMEPAD_B))
		this->Remove();
}

void GameWait::Draw(RenderingPass pass)
{
	const auto& dc = this->graphics_->device_context_;

	switch (pass)
	{
	case RenderingPass::UnableDepthStencil:
	{
		this->graphics_->SetBlendState(BlendState::AlphaToCoverage);

		dc->VSSetShader(this->data_->GetVertexShader(hlsl), nullptr, 0);
		dc->PSSetShader(this->data_->GetPixelShader(hlsl), nullptr, 0);
		dc->IASetInputLayout(this->data_->GetInputLayout(input_layout));
		dc->PSSetSamplers(0, 1, this->data_->GetSamplerState(sampler_state));
		(!XInput::Use())
			? dc->PSSetShaderResources(0, 1, this->data_->GetTexture(texture_keyboard))
			: dc->PSSetShaderResources(0, 1, this->data_->GetTexture(texture_xinput));
		dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		this->GetComponent<DrawIndexed>()->Draw(this->graphics_, this->constant_buffer_, this->data_->GetVertexBuffers(this->tag_), this->data_->GetIndexBuffers(this->tag_), this->data_->GetIndexSize(this->tag_));

		this->graphics_->SetBlendState(BlendState::Default);
	}
	break;
	}
}
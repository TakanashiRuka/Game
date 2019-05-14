#include "..\entity\time_up.h"

#include "..\graphics\directx11.h"
#include "..\framework\data.h"
#include "..\model\model_import.h"
#include "..\component\draw\draw_indexed.h"
#include "..\framework\game.h"
#include "..\framework\window.h"
#include "..\scene\result.h"
#include "..\entity\press.h"
#include "..\entity\key_button.h"
#include "..\input\keyboard.h"
#include "..\input\x_input.h"
#include "..\sound\x_audio2.h"

constexpr char* hlsl = "ui";
constexpr InputLayoutType input_layout = InputLayoutType::Simple;
constexpr SamplerStateType sampler_state = SamplerStateType::Simple;
constexpr Texture texture = Texture::T_Time_up;

TimeUp::TimeUp(Scene* parent) : Entity(parent)
{
	this->tag_ = "time up";

	this->ui_y_ = -Window::GetHeight<float>();
	auto model = ModelImport::UIData(0, this->ui_y_, Window::GetWidth<float>(), Window::GetHeight<float>(), 0, 0, 1, 1);

	this->data_->CreateVertexShaderAndInputLayout(hlsl, input_layout);
	this->data_->CreatePixelShader(hlsl);
	this->data_->CreateConstantBuffer(this->constant_buffer_, ConstantBufferType::Simple);
	this->data_->CreateVertexBuffers(this->tag_, VertexBufferType::UI, model);
	this->data_->CreateIndexBuffers(this->tag_, model);
	this->data_->CreateSamplerState(sampler_state);
	this->data_->CreateTexture(texture);

	this->AddComponent<DrawIndexed>(model.meshes.size());

	XAudio2::StopSourceVoice("forest");
}

TimeUp::~TimeUp()
{

}

void TimeUp::Update()
{
	if (!this->time_up_)
	{
		this->ui_y_ += Window::GetHeight<float>() * static_cast<float>(Game::GetInstance()->GetRunningTime()) * 2.5f;
		if (this->ui_y_ > 0)
		{
			this->ui_y_ = 0;
			this->time_up_ = true;
		}
		auto model = ModelImport::UIData(0, this->ui_y_, Window::GetWidth<float>(), Window::GetHeight<float>(), 0, 0, 1, 1);
		this->data_->UpdateVertexBuffers(this->tag_, model);
	}
	else if (!this->add_entity_)
	{
		this->running_time_ += Game::GetInstance()->GetRunningTime();
		if (this->running_time_ > 0.5)
		{
			this->parent_->AddEntity<Press>();
			this->parent_->AddEntity<KeyButton>();
			this->add_entity_ = true;
		}
	}

	if (this->add_entity_)
		if (KeyBoard::Trigger(VK_RETURN) || XInput::Trigger(XINPUT_GAMEPAD_B))
			Game::GetInstance()->SetScene<Result>();
}

void TimeUp::Draw(RenderingPass pass)
{
	const auto& dc = this->graphics_->device_context_;

	switch (pass)
	{
	case RenderingPass::UnableDepthStencil:
	{
		dc->VSSetShader(this->data_->GetVertexShader(hlsl), nullptr, 0);
		dc->PSSetShader(this->data_->GetPixelShader(hlsl), nullptr, 0);
		dc->IASetInputLayout(this->data_->GetInputLayout(input_layout));
		dc->PSSetSamplers(0, 1, this->data_->GetSamplerState(sampler_state));
		dc->PSSetShaderResources(0, 1, this->data_->GetTexture(texture));
		dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		this->GetComponent<DrawIndexed>()->Draw(this->graphics_, this->constant_buffer_, this->data_->GetVertexBuffers(this->tag_), this->data_->GetIndexBuffers(this->tag_), this->data_->GetIndexSize(this->tag_));
	}
	break;
	}
}
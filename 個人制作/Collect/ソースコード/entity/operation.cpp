#include "..\entity\operation.h"

#include "..\graphics\directx11.h"
#include "..\framework\data.h"
#include "..\model\model_import.h"
#include "..\component\draw\draw_indexed.h"
#include "..\framework\window.h"
#include "..\input\keyboard.h"
#include "..\input\x_input.h"
#include "..\sound\x_audio2.h"

constexpr char* hlsl = "ui";
constexpr InputLayoutType input_layout = InputLayoutType::Simple;
constexpr SamplerStateType sampler_state = SamplerStateType::Simple;
constexpr Texture texture_keyboard = Texture::T_Operation_;
constexpr Texture texture_xinput = Texture::T_Operation;

Operation::Operation(Scene* parent) : Entity(parent)
{
	this->tag_ = "operation";

	auto model = ModelImport::UIData(Window::GetWidth<float>() / 2 - 600, Window::GetHeight<float>() / 2 - 130, 552, 260, 0, 0, 1, 1);

	this->data_->CreateVertexShaderAndInputLayout(hlsl, input_layout);
	this->data_->CreatePixelShader(hlsl);
	this->data_->CreateVertexBuffers(this->tag_, VertexBufferType::UI, model);
	this->data_->CreateIndexBuffers(this->tag_, model);
	this->data_->CreateSamplerState(sampler_state);
	this->data_->CreateTexture(texture_keyboard);
	this->data_->CreateTexture(texture_xinput);

	this->AddComponent<DrawIndexed>(model.meshes.size());

	XAudio2::AddSourceVoice("title", SoundType::BGM);
	XAudio2::StartSourceVoice("title");
}

Operation::~Operation()
{

}

void Operation::Update()
{
	if (KeyBoard::Trigger(VK_RETURN) || XInput::Trigger(XINPUT_GAMEPAD_B))
	{
		XAudio2::StopSourceVoice("title");
		this->Remove();
	}
}

void Operation::Draw(RenderingPass pass)
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
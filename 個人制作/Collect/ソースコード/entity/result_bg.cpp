#include "..\entity\result_bg.h"

#include "..\graphics\directx11.h"
#include "..\framework\data.h"
#include "..\model\model_import.h"
#include "..\component\draw\draw_indexed.h"
#include "..\framework\game.h"
#include "..\framework\window.h"
#include "..\framework\scene.h"
#include "..\entity\result_high_score.h"
#include "..\entity\result_score.h"
#include "..\entity\high_score_update.h"
#include "..\entity\change_title.h"
#include "..\entity\press.h"
#include "..\entity\key_button.h"
#include "..\input\keyboard.h"
#include "..\input\x_input.h"
#include "..\sound\x_audio2.h"

constexpr char* hlsl = "ui";
constexpr InputLayoutType input_layout = InputLayoutType::Simple;
constexpr SamplerStateType sampler_state = SamplerStateType::Simple;
constexpr Texture texture = Texture::T_Result;

ResultBG::ResultBG(Scene* parent) : Entity(parent)
{
	this->tag_ = "result bg";

	auto model = ModelImport::UIData(0, 0, Window::GetWidth<float>(), Window::GetHeight<float>(), 0, 0, 1, 1);

	this->data_->CreateVertexShaderAndInputLayout(hlsl, input_layout);
	this->data_->CreatePixelShader(hlsl);
	this->data_->CreateConstantBuffer(this->constant_buffer_, ConstantBufferType::Simple);
	this->data_->CreateVertexBuffers(this->tag_, VertexBufferType::UI, model);
	this->data_->CreateIndexBuffers(this->tag_, model);
	this->data_->CreateSamplerState(sampler_state);
	this->data_->CreateTexture(texture);

	this->AddComponent<DrawIndexed>(model.meshes.size());

	XAudio2::AddSourceVoice("result", SoundType::BGM);
	XAudio2::StartSourceVoice("result");
}

ResultBG::~ResultBG()
{

}

void ResultBG::Update()
{
	Entity* high_score = nullptr;
	Entity* score = nullptr;
	for (auto& entity : this->parent_->GetEntities())
	{
		if (entity->FindWithTag("result high score"))
			high_score = entity;
		else if (entity->FindWithTag("result score"))
			score = entity;
	}

	if (static_cast<ResultScore*>(score)->GetScore() < static_cast<ResultScore*>(score)->GetSaveDataScore())
		static_cast<ResultScore*>(score)->GetScore() += 10;
	else if (!this->skip_ && this->late_)
	{
		this->parent_->AddEntity<Press>();
		this->parent_->AddEntity<KeyButton>();
		this->skip_ = true;
	}

	if (static_cast<ResultHighScore*>(high_score)->GetHighScore() < static_cast<ResultScore*>(score)->GetScore())
	{
		static_cast<ResultHighScore*>(high_score)->GetHighScore() = static_cast<ResultScore*>(score)->GetScore();

		if (!this->high_score_update_)
		{
			this->parent_->AddEntity<HighScoreUpdate>();

			this->high_score_update_ = true;
		}
	}

	if (this->skip_ && (KeyBoard::Trigger(VK_RETURN) || XInput::Trigger(XINPUT_GAMEPAD_B)))
		this->parent_->AddEntity<ChangeTitle>();

	if (!this->skip_ && (KeyBoard::Trigger(VK_RETURN) || XInput::Trigger(XINPUT_GAMEPAD_B)))
	{
		if (static_cast<ResultHighScore*>(high_score)->GetHighScore() < static_cast<ResultScore*>(score)->GetSaveDataScore())
		{
			static_cast<ResultHighScore*>(high_score)->GetHighScore() = static_cast<ResultScore*>(score)->GetScore() = static_cast<ResultScore*>(score)->GetSaveDataScore();

			if (!this->high_score_update_)
			{
				this->parent_->AddEntity<HighScoreUpdate>();

				this->high_score_update_ = true;
			}
		}
		else
			static_cast<ResultScore*>(score)->GetScore() = static_cast<ResultScore*>(score)->GetSaveDataScore();

		this->parent_->AddEntity<Press>();
		this->parent_->AddEntity<KeyButton>();
		this->skip_ = true;
	}

	if (!this->late_)
		this->late_ = true;
}

void ResultBG::Draw(RenderingPass pass)
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
#include "..\entity\title_name.h"

#include "..\graphics\directx11.h"
#include "..\framework\data.h"
#include "..\model\model_import.h"
#include "..\component\draw\draw_indexed.h"
#include "..\framework\game.h"
#include "..\framework\window.h"
#include "..\framework\scene.h"
#include "..\entity\mesh_field.h"
#include "..\entity\bill_board_animation.h"
#include "..\entity\title_fruit.h"
#include "..\component\collider\sphere_collider.h"
#include "..\entity\change_forest.h"
#include "..\entity\color.h"
#include "..\input\keyboard.h"
#include "..\input\x_input.h"
#include "..\sound\x_audio2.h"

constexpr char* hlsl = "ui";
constexpr InputLayoutType input_layout = InputLayoutType::Simple;
constexpr SamplerStateType sampler_state = SamplerStateType::Simple;
constexpr Texture texture = Texture::T_Title;

TitleName::TitleName(Scene* parent) : Entity(parent)
{
	this->tag_ = "title name";

	auto model = ModelImport::UIData((Window::GetWidth<float>() - 460) / 2, 100, 460, 125, 0, 0, 1, 1);

	this->data_->CreateVertexShaderAndInputLayout(hlsl, input_layout);
	this->data_->CreatePixelShader(hlsl);
	this->data_->CreateConstantBuffer(this->constant_buffer_, ConstantBufferType::Simple);
	this->data_->CreateVertexBuffers(this->tag_, VertexBufferType::UI, model);
	this->data_->CreateIndexBuffers(this->tag_, model);
	this->data_->CreateSamplerState(sampler_state);
	this->data_->CreateTexture(texture);

	this->AddComponent<DrawIndexed>(model.meshes.size());

	XAudio2::AddSourceVoice("title", SoundType::BGM);
	XAudio2::StartSourceVoice("title");
}

TitleName::~TitleName()
{

}

void TitleName::Update()
{
	this->running_time_ += Game::GetInstance()->GetRunningTime();
	if (this->running_time_ > 1)
	{
		for (auto& entity : this->parent_->GetEntities())
		{
			if (entity->FindWithTag("title fruit"))
			{
				if (entity->HasComponent<SphereCollider>())
				{
					entity->Remove();
					auto position = DirectX::XMFLOAT3(static_cast<float>(field_width) / 2, 0.5f, static_cast<float>(field_height) / 2);
					this->parent_->AddEntity<BillBoardAnimation>(BillBoardAnimationType::Blast, position, static_cast<TitleFruit*>(entity)->GetOutLineColor());
				}
			}
		}
		this->parent_->AddEntity<TitleFruit>();
		this->running_time_ = 0;
	}

	if (KeyBoard::Trigger(VK_RETURN) || XInput::Trigger(XINPUT_GAMEPAD_B))
		this->parent_->AddEntity<ChangeForest>();
}

void TitleName::Draw(RenderingPass pass)
{
	const auto& dc = this->graphics_->device_context_;

	switch (pass)
	{
	case RenderingPass::UnableDepthStencil:
	{
		this->graphics_->SetBlendState(BlendState::AlphaToCoverage);

		for (auto& entity : this->parent_->GetEntities())
			if (entity->FindWithTag("color"))
				static_cast<Color*>(entity)->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));

		dc->VSSetShader(this->data_->GetVertexShader(hlsl), nullptr, 0);
		dc->PSSetShader(this->data_->GetPixelShader(hlsl), nullptr, 0);
		dc->IASetInputLayout(this->data_->GetInputLayout(input_layout));
		dc->PSSetSamplers(0, 1, this->data_->GetSamplerState(sampler_state));
		dc->PSSetShaderResources(0, 1, this->data_->GetTexture(texture));
		dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		this->GetComponent<DrawIndexed>()->Draw(this->graphics_, this->constant_buffer_, this->data_->GetVertexBuffers(this->tag_), this->data_->GetIndexBuffers(this->tag_), this->data_->GetIndexSize(this->tag_));

		this->graphics_->SetBlendState(BlendState::Default);
	}
	break;
	}
}
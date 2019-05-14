#include "..\entity\bomb.h"

#include "..\graphics\directx11.h"
#include "..\framework\data.h"
#include "..\model\model_import.h"
#include "..\component\draw\draw_indexed.h"
#include "..\framework\game.h"
#include "..\framework\window.h"
#include "..\framework\scene.h"
#include "..\input\keyboard.h"
#include "..\entity\bill_board_animation.h"
#include "..\entity\score.h"
#include "..\entity\fruit.h"
#include "..\component\collider\sphere_collider.h"

constexpr char* hlsl = "ui";
constexpr InputLayoutType input_layout = InputLayoutType::Simple;
constexpr SamplerStateType sampler_state = SamplerStateType::Simple;
constexpr Texture texture = Texture::T_Fade;

Bomb::Bomb(Scene* parent) : Entity(parent)
{
	this->tag_ = "bomb";

	auto model = ModelImport::UIData(Window::GetWidth<float>() - 100, Window::GetHeight<float>() - 100, 100, 100, 0, 0, 1, 1);

	this->data_->CreateVertexShaderAndInputLayout(hlsl, input_layout);
	this->data_->CreatePixelShader(hlsl);
	this->data_->CreateConstantBuffer(this->constant_buffer_, ConstantBufferType::Simple);
	this->data_->CreateVertexBuffers(this->tag_, VertexBufferType::UI, model);
	this->data_->CreateIndexBuffers(this->tag_, model);
	this->data_->CreateSamplerState(sampler_state);
	this->data_->CreateTexture(texture);

	this->AddComponent<DrawIndexed>(model.meshes.size());
}

Bomb::~Bomb()
{

}

void Bomb::Update()
{
	if (KeyBoard::Trigger('M'))
	{
		Entity* score = nullptr;
		for (auto& entity : this->parent_->GetEntities())
			if (entity->FindWithTag("score"))
				score = entity;

		for (auto& entity : this->parent_->GetEntities())
		{
			if (entity->FindWithTag("fruit"))
			{
				static_cast<Score*>(score)->AddScore(10);
				auto fruit = static_cast<Fruit*>(entity);
				this->parent_->AddEntity<BillBoardAnimation>(BillBoardAnimationType::Blast, fruit->GetComponent<SphereCollider>()->GetPosition(), fruit->GetOutLineColor());
				this->parent_->AddEntity<Fruit>();
				entity->Remove();
			}
		}
	}
}

void Bomb::Draw(RenderingPass pass)
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
		dc->PSSetShaderResources(0, 1, this->data_->GetTexture(texture));
		dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		this->GetComponent<DrawIndexed>()->Draw(this->graphics_, this->constant_buffer_, this->data_->GetVertexBuffers(this->tag_), this->data_->GetIndexBuffers(this->tag_), this->data_->GetIndexSize(this->tag_));

		this->graphics_->SetBlendState(BlendState::Default);
	}
	break;
	}
}
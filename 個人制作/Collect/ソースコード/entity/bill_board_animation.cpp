#include "..\entity\bill_board_animation.h"

#include "..\graphics\directx11.h"
#include "..\framework\data.h"
#include "..\model\model_import.h"
#include "..\component\draw\draw_indexed.h"
#include "..\framework\window.h"
#include "..\framework\scene.h"
#include "..\framework\collision_math.h"
#include "..\component\camera\tps.h"
#include "..\entity\color.h"
#include "..\entity\title_camera.h"
#include "..\component\collider\sphere_collider.h"
#include "..\entity\mesh_field.h"
#include "..\sound\x_audio2.h"

constexpr char* hlsl = "simple_bill_board";
constexpr InputLayoutType input_layout = InputLayoutType::Simple;
constexpr SamplerStateType sampler_state = SamplerStateType::Simple;

BillBoardAnimation::BillBoardAnimation(Scene* parent, BillBoardAnimationType type, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT4 color) : Entity(parent), type_(type), pos_(pos), color_(color)
{
	switch (this->type_)
	{
	case BillBoardAnimationType::Blast:
	{
		this->tag_ = "blast";
		this->texture_ = Texture::T_Blast;

		this->texture_animation_max_ = 10;
		this->pos_.y += 0.1f;
		this->width_ = 0.4f;
		this->height_ = 0.4f;
		this->uv_x_ = 0;
		this->uv_y_ = 0;
		this->uv_w_ = 1.0f / this->texture_animation_max_;
		this->uv_h_ = 1;
	}
	break;

	case BillBoardAnimationType::Impact:
	{
		this->tag_ = "impact";
		this->texture_ = Texture::T_Impact;

		this->texture_animation_max_ = 7;
		this->width_ = 0.4f;
		this->height_ = 0.4f;
		this->uv_x_ = 0;
		this->uv_y_ = 0;
		this->uv_w_ = 1.0f / this->texture_animation_max_;
		this->uv_h_ = 1;
	}
	break;
	}

	auto model = ModelImport::BillBoardData(1, 1, 0, 0, 0, 0);

	this->data_->CreateVertexShaderAndInputLayout(hlsl, input_layout);
	this->data_->CreatePixelShader(hlsl);
	this->data_->CreateConstantBuffer(this->constant_buffer_, ConstantBufferType::Simple);
	this->data_->CreateVertexBuffers(this->tag_, VertexBufferType::UI, model);
	this->data_->CreateIndexBuffers(this->tag_, model);
	this->data_->CreateSamplerState(sampler_state);
	this->data_->CreateTexture(this->texture_);

	this->AddComponent<DrawIndexed>(model.meshes.size());

	XAudio2::AddSourceVoice(this->tag_, SoundType::SE);
	XAudio2::StartSourceVoice(this->tag_);
}

BillBoardAnimation::~BillBoardAnimation()
{

}

void BillBoardAnimation::Update()
{
	switch (this->type_)
	{
	case BillBoardAnimationType::Blast:
	case BillBoardAnimationType::Impact:
	{
		if (this->wait_)
		{
			this->wait_ = false;

			this->uv_x_ += this->uv_w_;
			++this->texture_animation_;
		}
		else
		{
			this->wait_ = true;

			if (this->texture_animation_ == this->texture_animation_max_)
				this->Remove();
		}
	}
	break;
	}

	CBufferSimple cb;
	for (auto& entity : this->parent_->GetEntities())
	{
		if (entity->FindWithTag("player"))
		{
			if (entity->HasComponent<TPS>())
				cb.world = DirectX::XMMatrixInverse(nullptr, entity->GetComponent<TPS>()->GetView());
			break;
		}
		if (entity->FindWithTag("title camera"))
		{
			cb.world = DirectX::XMMatrixRotationY(static_cast<TitleCamera*>(entity)->GetRotY()) * DirectX::XMMatrixRotationX(static_cast<TitleCamera*>(entity)->GetRotX())
				* DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(0, 0, -1.4f, 0), DirectX::XMVectorSet(0, 0, 0, 0), DirectX::XMVectorSet(0, 1, 0, 0));
			cb.world = DirectX::XMMatrixInverse(nullptr, cb.world);
			break;
		}
	}
	cb.world.r[3].m128_f32[0] = cb.world.r[3].m128_f32[1] = cb.world.r[3].m128_f32[2] = 0;
	cb.world *= DirectX::XMMatrixTranslation(this->pos_.x, this->pos_.y, this->pos_.z);
	this->data_->UpdateConstantBuffer(this->constant_buffer_, &cb);

	DirectX::XMMATRIX view;
	for (auto& entity : this->parent_->GetEntities())
	{
		if (entity->FindWithTag("player"))
			if (entity->HasComponent<TPS>())
			{
				view = entity->GetComponent<TPS>()->GetView();
				if (CollisionMath::ViewFrustum(this->pos_, 0.1f, view, 60, (float)init_width / init_height, 0.1f, static_cast<float>(field_width / 2)))
					this->draw_ = true;
				else
					this->draw_ = false;
				break;
			}

		if (entity->FindWithTag("title camera"))
		{
			this->draw_ = true;
		}
	}

}

void BillBoardAnimation::Draw(RenderingPass pass)
{
	if (this->draw_)
	{
		const auto& dc = this->graphics_->device_context_;

		switch (pass)
		{
		case RenderingPass::Base:
		{
			Entity* color = nullptr;
			for (auto& entity : this->parent_->GetEntities())
			{
				if (entity->FindWithTag("color"))
				{
					color = entity;
					static_cast<Color*>(color)->SetColor(this->color_);
				}
			}

			auto model = ModelImport::BillBoardData(this->width_, this->height_, this->uv_x_, this->uv_y_, this->uv_w_, this->uv_h_);
			this->data_->UpdateVertexBuffers(this->tag_, model);

			this->graphics_->SetBlendState(BlendState::AlphaToCoverage);

			dc->VSSetShader(this->data_->GetVertexShader(hlsl), nullptr, 0);
			dc->PSSetShader(this->data_->GetPixelShader(hlsl), nullptr, 0);
			dc->IASetInputLayout(this->data_->GetInputLayout(input_layout));
			dc->PSSetSamplers(0, 1, this->data_->GetSamplerState(sampler_state));
			dc->PSSetShaderResources(0, 1, this->data_->GetTexture(this->texture_));
			dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			this->GetComponent<DrawIndexed>()->Draw(this->graphics_, this->constant_buffer_, this->data_->GetVertexBuffers(this->tag_), this->data_->GetIndexBuffers(this->tag_), this->data_->GetIndexSize(this->tag_));

			static_cast<Color*>(color)->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));

			this->graphics_->SetBlendState(BlendState::Default);
		}
		break;
		}
	}
}
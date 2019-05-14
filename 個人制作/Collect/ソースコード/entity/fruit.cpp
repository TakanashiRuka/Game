#include "..\entity\fruit.h"

#include "..\graphics\directx11.h"
#include "..\framework\data.h"
#include "..\model\model_import.h"
#include "..\component\draw\draw_indexed.h"
#include "..\component\collider\sphere_collider.h"
#include "..\framework\window.h"
#include "..\framework\scene.h"
#include "..\framework\math.h"
#include "..\framework\collision_math.h"
#include "..\entity\bill_board_animation.h"
#include "..\entity\mesh_field.h"
#include "..\entity\fruit_index.h"
#include "..\component\camera\tps.h"
#include "..\framework\debug.h"

#include <random>

constexpr char* depth = "simple_pcf_depth";
constexpr char* base_and_outline = "simple_pcf_base_and_outline";
constexpr InputLayoutType input_layout = InputLayoutType::Simple;
constexpr SamplerStateType sampler_state = SamplerStateType::Simple;
constexpr SamplerStateType sampler_state_shadow = SamplerStateType::Shadow;

using namespace DirectX;

Fruit::Fruit(Scene* parent) : Entity(parent)
{
	this->tag_ = "fruit";

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<> random_fruit(FruitType::Apple, FruitType::Strawberry);
	this->type_ = random_fruit(mt);
	switch (this->type_)
	{
	case FruitType::Apple:
	{
		this->name_ = "apple";
		this->texture_ = Texture::T_Apple;
		this->out_line_color_ = { 1, 0, 0, 1 };
		this->scaling_ = 3;
	}
	break;

	case FruitType::Banana:
	{
		this->name_ = "banana";
		this->texture_ = Texture::T_Banana;
		this->out_line_color_ = { 1, 1, 0, 1 };
		this->scaling_ = 3;
	}
	break;

	case FruitType::Kiwi:
	{
		this->name_ = "kiwi";
		this->texture_ = Texture::T_Kiwi;
		this->out_line_color_ = { 0.6f, 0.6f, 0, 1 };
		this->scaling_ = 3.5f;
	}
	break;

	case FruitType::Pear:
	{
		this->name_ = "pear";
		this->texture_ = Texture::T_Pear;
		this->out_line_color_ = { 0.8f, 1, 0, 1 };
		this->scaling_ = 3;
	}
	break;

	case FruitType::Strawberry:
	{
		this->name_ = "strawberry";
		this->texture_ = Texture::T_Strawberry;
		this->out_line_color_ = { 1, 0, 0, 1 };
		this->scaling_ = 5;
	}
	break;
	}

	auto model = ModelImport::ModelData(this->name_);
	this->data_->CreateVertexShaderAndInputLayout(depth, input_layout);
	this->data_->CreatePixelShader(depth);
	this->data_->CreateVertexShaderAndInputLayout(base_and_outline, input_layout);
	this->data_->CreateGeometryShader(base_and_outline);
	this->data_->CreatePixelShader(base_and_outline);
	this->data_->CreateConstantBuffer(this->constant_buffer_, ConstantBufferType::SimpleShadowAndOutline);
	this->data_->CreateVertexBuffers(this->name_, VertexBufferType::Model, model);
	this->data_->CreateIndexBuffers(this->name_, model);
	this->data_->CreateSamplerState(sampler_state);
	this->data_->CreateSamplerState(sampler_state_shadow);
	this->data_->CreateTexture(this->texture_);

	this->AddComponent<DrawIndexed>(model.meshes.size());
	this->AddComponent<SphereCollider>();

	std::uniform_real_distribution<float> random_rot_y(0, 359);
	this->rotation_y_ = random_rot_y(mt);

	this->field_y_ = 0.4f;
	Entity* fruit_index = nullptr;
	for (auto& entity : this->parent_->GetEntities())
		if (entity->FindWithTag("fruit index"))
			fruit_index = entity;
	int index = static_cast<FruitIndex*>(fruit_index)->GetIndex();

	this->GetComponent<SphereCollider>()->GetPosition() = { static_cast<float>(Math::HaltonSeq(index, 3) * (field_width - 6)) + 3, 3 + this->field_y_, static_cast<float>(Math::HaltonSeq(index, 2) * (field_height - 6)) + 3 };
	this->GetComponent<SphereCollider>()->GetRadius() = 0.1f;

	static_cast<FruitIndex*>(fruit_index)->AddIndex();
}

Fruit::~Fruit()
{

}

void Fruit::Update()
{
	auto& position = this->GetComponent<SphereCollider>()->GetPosition();
	if (position.y > this->field_y_)
		position.y -= 0.15f;
	else if (!this->fall_)
	{
		this->parent_->AddEntity<BillBoardAnimation>(BillBoardAnimationType::Impact, position, DirectX::XMFLOAT4(1, 1, 1, 1));
		this->fall_ = true;
	}
	++this->rotation_y_;
	if (this->rotation_y_ >= 360)
		this->rotation_y_ = 0;

	CBufferSimpleShadowAndOutline cb;
	cb.world = XMMatrixScaling(this->scaling_, this->scaling_, this->scaling_) * XMMatrixRotationY(XMConvertToRadians(this->rotation_y_)) * XMMatrixTranslation(position.x, position.y, position.z);
	cb.texel_size = DirectX::XMFLOAT4(1.0f / (1024 * depth_texture_size), 1.0f / (1024 * depth_texture_size), 0, 0);
	cb.out_line_color = this->out_line_color_;
	cb.border_width = 0.003f;
	this->data_->UpdateConstantBuffer(this->constant_buffer_, &cb);

	XMMATRIX view;
	for (auto& entity : this->parent_->GetEntities())
		if (entity->FindWithTag("player"))
			if (entity->HasComponent<TPS>())
			{
				view = entity->GetComponent<TPS>()->GetView();
				if (CollisionMath::ViewFrustum(this->GetComponent<SphereCollider>()->GetPosition(), this->GetComponent<SphereCollider>()->GetRadius() * 2, view, 60, (float)init_width / init_height, 0.1f, static_cast<float>(field_width / 2)))
					this->draw_ = true;
				else
					this->draw_ = false;
				break;
			}
}

void Fruit::Draw(RenderingPass pass)
{
	if (this->draw_)
	{
		const auto& dc = this->graphics_->device_context_;

		switch (pass)
		{
		case RenderingPass::Depth:
		{
			dc->VSSetShader(this->data_->GetVertexShader(depth), nullptr, 0);
			dc->PSSetShader(this->data_->GetPixelShader(depth), nullptr, 0);
			dc->IASetInputLayout(this->data_->GetInputLayout(input_layout));
			dc->PSSetSamplers(1, 1, this->data_->GetSamplerState(sampler_state_shadow));
			dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			this->GetComponent<DrawIndexed>()->Draw(this->graphics_, this->constant_buffer_, this->data_->GetVertexBuffers(this->name_), this->data_->GetIndexBuffers(this->name_), this->data_->GetIndexSize(this->name_));
		}
		break;

		case RenderingPass::Base:
		{
			this->graphics_->SetBlendState(BlendState::AlphaToCoverage);

			dc->VSSetShader(this->data_->GetVertexShader(base_and_outline), nullptr, 0);
			dc->GSSetShader(this->data_->GetGeometryShader(base_and_outline), nullptr, 0);
			dc->PSSetShader(this->data_->GetPixelShader(base_and_outline), nullptr, 0);
			dc->IASetInputLayout(this->data_->GetInputLayout(input_layout));
			dc->PSSetSamplers(0, 1, this->data_->GetSamplerState(sampler_state));
			dc->PSSetSamplers(1, 1, this->data_->GetSamplerState(sampler_state_shadow));
			dc->PSSetShaderResources(0, 1, this->data_->GetTexture(this->texture_));
			dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			this->GetComponent<DrawIndexed>()->Draw(this->graphics_, this->constant_buffer_, this->data_->GetVertexBuffers(this->name_), this->data_->GetIndexBuffers(this->name_), this->data_->GetIndexSize(this->name_));

			dc->GSSetShader(nullptr, nullptr, 0);

			this->graphics_->SetBlendState(BlendState::Default);
		}
		break;

		case RenderingPass::Debug:
		{
			Debug::DrawObject(this->tag_, this->GetComponent<SphereCollider>()->GetPosition());
		}
		break;
		}
	}
}



Texture Fruit::GetUseTexture()
{
	return this->texture_;
}

DirectX::XMFLOAT4 Fruit::GetOutLineColor()
{
	return this->out_line_color_;
}
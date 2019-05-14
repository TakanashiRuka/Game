#include "..\entity\theme.h"

#include "..\graphics\directx11.h"
#include "..\framework\data.h"
#include "..\model\model_import.h"
#include "..\component\draw\draw_indexed.h"
#include "..\framework\scene.h"
#include "..\entity\fruit.h"
#include "..\component\movement.h"
#include "..\component\camera\tps.h"

#include <random>

constexpr char* hlsl = "simple";
constexpr InputLayoutType input_layout = InputLayoutType::Simple;
constexpr SamplerStateType sampler_state = SamplerStateType::Simple;

using namespace DirectX;

Theme::Theme(Scene* parent) : Entity(parent)
{
	this->tag_ = "theme";

	this->ChangeThemeTexture();

	this->data_->CreateVertexShaderAndInputLayout(hlsl, input_layout);
	this->data_->CreatePixelShader(hlsl);
	this->data_->CreateConstantBuffer(this->constant_buffer_, ConstantBufferType::Simple);
	this->data_->CreateSamplerState(sampler_state);
	this->data_->CreateTexture(this->theme_);
}

Theme::~Theme()
{

}

void Theme::Update()
{
	++this->rot_y_;
	if (this->rot_y_ >= 360)
		this->rot_y_ = 0;
}

void Theme::Draw(RenderingPass pass)
{
	XMFLOAT3 pos;
	for (auto& entity : this->parent_->GetEntities())
	{
		if (entity->FindWithTag("player"))
		{
			if (entity->HasComponent<Movement>())
				pos = entity->GetComponent<Movement>()->GetPos();

			break;
		}
	}

	CBufferSimple cb;
	cb.world = XMMatrixScaling(this->scaling_, this->scaling_, this->scaling_) * XMMatrixRotationY(XMConvertToRadians(this->rot_y_)) * XMMatrixTranslation(pos.x, pos.y + 1, pos.z);
	this->data_->UpdateConstantBuffer(this->constant_buffer_, &cb);

	const auto& dc = this->graphics_->device_context_;

	switch (pass)
	{
	case RenderingPass::Base:
	{
		this->graphics_->SetBlendState(BlendState::AlphaToCoverage);

		if (!this->HasComponent<DrawIndexed>())
		{
			auto model = ModelImport::ModelData(this->name_);
			this->data_->CreateVertexBuffers(this->name_, VertexBufferType::Model, model);
			this->data_->CreateIndexBuffers(this->name_, model);
			this->AddComponent<DrawIndexed>(model.meshes.size());
		}

		dc->VSSetShader(this->data_->GetVertexShader(hlsl), nullptr, 0);
		dc->PSSetShader(this->data_->GetPixelShader(hlsl), nullptr, 0);
		dc->IASetInputLayout(this->data_->GetInputLayout(input_layout));
		dc->PSSetSamplers(0, 1, this->data_->GetSamplerState(sampler_state));
		dc->PSSetShaderResources(0, 1, this->data_->GetTexture(this->theme_));
		dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		this->GetComponent<DrawIndexed>()->Draw(this->graphics_, this->constant_buffer_, this->data_->GetVertexBuffers(this->name_), this->data_->GetIndexBuffers(this->name_), this->data_->GetIndexSize(this->name_));

		this->graphics_->SetBlendState(BlendState::Default);
	}
	break;
	}
}

Texture Theme::GetThemeTexture()
{
	return this->theme_;
}

void Theme::ChangeThemeTexture()
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<> random_fruit(FruitType::Apple, FruitType::Strawberry);
	std::string old_name = this->name_;

	for (;;)
	{
		switch (random_fruit(mt))
		{
		case FruitType::Apple:
			this->name_ = "apple";
			this->theme_ = Texture::T_Apple;
			this->scaling_ = 3;
			break;

		case FruitType::Banana:
			this->name_ = "banana";
			this->theme_ = Texture::T_Banana;
			this->scaling_ = 3;
			break;

		case FruitType::Kiwi:
			this->name_ = "kiwi";
			this->theme_ = Texture::T_Kiwi;
			this->scaling_ = 3.5f;
			break;

		case FruitType::Pear:
			this->name_ = "pear";
			this->theme_ = Texture::T_Pear;
			this->scaling_ = 3;
			break;

		case FruitType::Strawberry:
			this->name_ = "strawberry";
			this->theme_ = Texture::T_Strawberry;
			this->scaling_ = 5;
			break;
		}
		if (old_name != this->name_)
			break;
	}

	this->scaling_ *= 0.8f;
	this->rot_y_ = 0;


	if (this->HasComponent<DrawIndexed>())
		this->RemoveComponent<DrawIndexed>();
}
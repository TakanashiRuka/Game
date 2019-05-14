#include "..\entity\change_title.h"

#include "..\graphics\directx11.h"
#include "..\framework\data.h"
#include "..\model\model_import.h"
#include "..\component\draw\draw_indexed.h"
#include "..\framework\game.h"
#include "..\framework\window.h"
#include "..\scene\title.h"
#include "..\entity\color.h"

constexpr char* hlsl = "ui";
constexpr InputLayoutType input_layout = InputLayoutType::Simple;
constexpr SamplerStateType sampler_state = SamplerStateType::Simple;
constexpr Texture texture = Texture::T_Fade;

ChangeTitle::ChangeTitle(Scene* parent) : Entity(parent)
{
	this->tag_ = "ct";

	auto model = ModelImport::UIData(0, 0, Window::GetWidth<float>(), Window::GetHeight<float>(), 0, 0, 1, 1);

	this->data_->CreateVertexShaderAndInputLayout(hlsl, input_layout);
	this->data_->CreatePixelShader(hlsl);
	this->data_->CreateConstantBuffer(this->constant_buffer_, ConstantBufferType::Simple);
	this->data_->CreateVertexBuffers(this->tag_, VertexBufferType::UI, model);
	this->data_->CreateIndexBuffers(this->tag_, model);
	this->data_->CreateSamplerState(sampler_state);
	this->data_->CreateTexture(texture);

	this->AddComponent<DrawIndexed>(model.meshes.size());
}

ChangeTitle::~ChangeTitle()
{

}

void ChangeTitle::Draw(RenderingPass pass)
{
	const auto& dc = this->graphics_->device_context_;

	switch (pass)
	{
	case RenderingPass::UnableDepthStencil:
	{
		this->graphics_->SetBlendState(BlendState::BlendEnable);

		Entity* color = nullptr;
		for (auto& entity : this->parent_->GetEntities())
		{
			if (entity->FindWithTag("color"))
			{
				color = entity;
				static_cast<Color*>(color)->SetColor(DirectX::XMFLOAT4(1, 1, 1, this->alpha_));
				this->alpha_ += 0.01f;
				if (this->alpha_ > 1)
					Game::GetInstance()->SetScene<Title>();
			}
		}

		dc->VSSetShader(this->data_->GetVertexShader(hlsl), nullptr, 0);
		dc->PSSetShader(this->data_->GetPixelShader(hlsl), nullptr, 0);
		dc->IASetInputLayout(this->data_->GetInputLayout(input_layout));
		dc->PSSetSamplers(0, 1, this->data_->GetSamplerState(sampler_state));
		dc->PSSetShaderResources(0, 1, this->data_->GetTexture(texture));
		dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		this->GetComponent<DrawIndexed>()->Draw(this->graphics_, this->constant_buffer_, this->data_->GetVertexBuffers(this->tag_), this->data_->GetIndexBuffers(this->tag_), this->data_->GetIndexSize(this->tag_));

		static_cast<Color*>(color)->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));

		this->graphics_->SetBlendState(BlendState::Default);
	}
	break;
	}
}
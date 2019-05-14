#include "..\entity\combo.h"

#include "..\graphics\directx11.h"
#include "..\framework\data.h"
#include "..\model\model_import.h"
#include "..\component\draw\draw_indexed.h"
#include "..\framework\window.h"
#include "..\framework\scene.h"
#include "..\component\camera\tps.h"
#include "..\component\movement.h"
#include "..\entity\color.h"
#include "..\framework\debug.h"

constexpr char* hlsl = "ui";
constexpr InputLayoutType input_layout = InputLayoutType::Simple;
constexpr SamplerStateType sampler_state = SamplerStateType::Simple;
constexpr Texture texture = Texture::T_Combo;

using namespace DirectX;

Combo::Combo(Scene* parent) : Entity(parent)
{
	this->tag_ = "combo";

	float width = 151;
	float height = 36;
	auto model = ModelImport::UIData(Window::GetWidth<float>() / 2 - width / 2 + 24, Window::GetHeight<float>() / 2 - height / 2, width, height, 0, 0, 1, 1);

	this->data_->CreateVertexShaderAndInputLayout(hlsl, input_layout);
	this->data_->CreatePixelShader(hlsl);
	this->data_->CreateVertexBuffers(this->tag_, VertexBufferType::UI, model);
	this->data_->CreateIndexBuffers(this->tag_, model);
	this->data_->CreateSamplerState(sampler_state);
	this->data_->CreateTexture(texture);

	this->AddComponent<DrawIndexed>(model.meshes.size());
}

Combo::~Combo()
{

}

void Combo::Draw(RenderingPass pass)
{
	const auto& dc = this->graphics_->device_context_;

	switch (pass)
	{
	case RenderingPass::UnableDepthStencil:
	{
		if (this->combo_cnt_ > 0)
		{
			this->graphics_->SetBlendState(BlendState::AlphaToCoverage);

			Entity* color = nullptr;
			for (auto& entity : this->parent_->GetEntities())
			{
				if (entity->FindWithTag("color"))
				{
					color = entity;
					static_cast<Color*>(color)->SetColor(DirectX::XMFLOAT4(1, 1, 1, this->alpha_));

					if (this->alpha_ > 0.5f)
						this->alpha_ -= 0.01f;
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
	}
	break;

	case RenderingPass::Debug:
	{
		XMFLOAT3 pos(static_cast<float>(this->combo_cnt_), 0, 0);
		Debug::DrawObject(this->tag_, pos);
	}
	break;
	}
}

void Combo::AddCombo()
{
	++this->combo_cnt_;
	this->alpha_ = 1;
}

unsigned int Combo::GetCombo()
{
	return this->combo_cnt_;
}

void Combo::ResetCombo()
{
	this->combo_cnt_ = 0;
}
#include "..\entity\ground.h"

#include "..\graphics\directx11.h"
#include "..\framework\data.h"
#include "..\model\model_import.h"
#include "..\component\draw\draw_indexed.h"
#include "..\framework\scene.h"
#include "..\entity\color.h"

constexpr char* hlsl = "simple_bill_board";
constexpr InputLayoutType input_layout = InputLayoutType::Simple;
constexpr SamplerStateType sampler_state = SamplerStateType::Simple;
constexpr Texture texture = Texture::T_Field;

constexpr char* name = "mesh_field";

Ground::Ground(Scene* parent) : Entity(parent)
{
	this->tag_ = "ground";

	this->data_->CreateVertexShaderAndInputLayout(hlsl, input_layout);
	this->data_->CreatePixelShader(hlsl);
	this->data_->CreateConstantBuffer(this->constant_buffer_, ConstantBufferType::Simple);
	this->data_->CreateSamplerState(sampler_state);
	this->data_->CreateTexture(texture);

	this->AddComponent<DrawIndexed>(1);

	CBufferSimple cb;
	cb.world = DirectX::XMMatrixIdentity();
	this->data_->UpdateConstantBuffer(this->constant_buffer_, &cb);
}

Ground::~Ground()
{

}

void Ground::Update()
{

}

void Ground::Draw(RenderingPass pass)
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
				float dark = 0.9f;
				static_cast<Color*>(color)->SetColor(DirectX::XMFLOAT4(dark, dark, dark, 1));
			}
		}

		dc->VSSetShader(this->data_->GetVertexShader(hlsl), nullptr, 0);
		dc->PSSetShader(this->data_->GetPixelShader(hlsl), nullptr, 0);
		dc->IASetInputLayout(this->data_->GetInputLayout(input_layout));
		dc->PSSetSamplers(0, 1, this->data_->GetSamplerState(sampler_state));
		dc->PSSetShaderResources(0, 1, this->data_->GetTexture(texture));
		dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		this->GetComponent<DrawIndexed>()->Draw(this->graphics_, this->constant_buffer_, this->data_->GetVertexBuffers(name), this->data_->GetIndexBuffers(name), this->data_->GetIndexSize(name));

		static_cast<Color*>(color)->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
	}
	break;
	}
}
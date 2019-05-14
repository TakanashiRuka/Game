#include "..\entity\sky_box.h"

#include "..\graphics\directx11.h"
#include "..\framework\data.h"
#include "..\model\model_import.h"
#include "..\component\draw\draw_indexed.h"
#include "..\framework\scene.h"
#include "..\component\movement.h"

#include <algorithm>

constexpr char* hlsl = "simple";
constexpr InputLayoutType input_layout = InputLayoutType::Simple;
constexpr SamplerStateType sampler_state = SamplerStateType::Simple;
constexpr Texture texture = Texture::T_Sky_box;

constexpr float scale = 100;
constexpr float rot = 90;

using namespace DirectX;

SkyBox::SkyBox(Scene* parent) : Entity(parent)
{
	this->tag_ = "sky_box";

	auto model = ModelImport::ModelData(this->tag_);

	this->data_->CreateVertexShaderAndInputLayout(hlsl, input_layout);
	this->data_->CreatePixelShader(hlsl);
	this->data_->CreateConstantBuffer(this->constant_buffer_, ConstantBufferType::Simple);
	this->data_->CreateVertexBuffers(this->tag_, VertexBufferType::Model, model);
	this->data_->CreateIndexBuffers(this->tag_, model);
	this->data_->CreateSamplerState(sampler_state);
	this->data_->CreateTexture(texture);

	this->AddComponent<DrawIndexed>(model.meshes.size());
}

SkyBox::~SkyBox()
{

}

void SkyBox::Always()
{
	const auto& entities = this->parent_->GetEntities();
	const auto& entity = std::find_if(entities.begin(), entities.end(), [&](Entity* e)
	{
		return e->HasComponent<Movement>();
	});
	const auto& pos = (*entity)->GetComponent<Movement>()->GetPos();

	CBufferSimple cb;
	cb.world = XMMatrixScaling(scale, scale, scale) * XMMatrixRotationX(XMConvertToRadians(-rot)) * XMMatrixRotationY(XMConvertToRadians(rot)) * XMMatrixTranslation(pos.x, pos.y, pos.z);
	this->data_->UpdateConstantBuffer(this->constant_buffer_, &cb);
}

void SkyBox::Draw(RenderingPass pass)
{
	const auto& dc = this->graphics_->device_context_;

	switch (pass)
	{
	case RenderingPass::Base:
	{
		dc->VSSetShader(this->data_->GetVertexShader(hlsl), nullptr, 0);
		dc->PSSetShader(this->data_->GetPixelShader(hlsl), nullptr, 0);
		dc->IASetInputLayout(this->data_->GetInputLayout(input_layout));
		dc->PSSetSamplers(0, 1, this->data_->GetSamplerState(sampler_state));
		dc->PSSetShaderResources(0, 1, this->data_->GetTexture(texture));
		dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		this->GetComponent<DrawIndexed>()->Draw(this->graphics_, this->constant_buffer_, this->data_->GetVertexBuffers(this->tag_), this->data_->GetIndexBuffers(this->tag_), this->data_->GetIndexSize(this->tag_));
	}
	break;
	}
}
#include "..\entity\mesh_field.h"

#include "..\graphics\directx11.h"
#include "..\framework\data.h"
#include "..\model\model_import.h"
#include "..\component\draw\draw_indexed.h"
#include "..\component\collider\mesh_collider.h"

constexpr char* depth = "simple_pcf_depth";
constexpr char* base_and_outline = "simple_pcf_base_and_outline";
constexpr InputLayoutType input_layout = InputLayoutType::Simple;
constexpr SamplerStateType sampler_state = SamplerStateType::Simple;
constexpr SamplerStateType sampler_state_shadow = SamplerStateType::Shadow;
constexpr Texture texture = Texture::T_Field;

using namespace DirectX;

MeshField::MeshField(Scene* parent) : Entity(parent)
{
	this->tag_ = "mesh_field";

	auto model = ModelImport::FieldData(field_width, field_height, 1, 1, 0.2f);

	this->data_->CreateVertexShaderAndInputLayout(depth, input_layout);
	this->data_->CreatePixelShader(depth);
	this->data_->CreateVertexShaderAndInputLayout(base_and_outline, input_layout);
	this->data_->CreatePixelShader(base_and_outline);
	this->data_->CreateConstantBuffer(this->constant_buffer_, ConstantBufferType::SimpleShadowAndOutline);
	this->data_->CreateVertexBuffers(this->tag_, VertexBufferType::MeshField, model);
	this->data_->CreateIndexBuffers(this->tag_, model);
	this->data_->CreateSamplerState(sampler_state);
	this->data_->CreateSamplerState(sampler_state_shadow);
	this->data_->CreateTexture(texture);

	this->AddComponent<MeshCollider>(model);
	this->AddComponent<DrawIndexed>(model.meshes.size());

	CBufferSimpleShadowAndOutline cb;
	cb.world = XMMatrixIdentity();
	cb.texel_size = XMFLOAT4(1.0f / (1024 * depth_texture_size), 1.0f / (1024 * depth_texture_size), 0, 0);
	this->data_->UpdateConstantBuffer(this->constant_buffer_, &cb);
}

MeshField::~MeshField()
{
}

void MeshField::Draw(RenderingPass pass)
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
		dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		this->GetComponent<DrawIndexed>()->Draw(this->graphics_, this->constant_buffer_, this->data_->GetVertexBuffers(this->tag_), this->data_->GetIndexBuffers(this->tag_), this->data_->GetIndexSize(this->tag_));
	}
	break;

	case RenderingPass::Base:
	{
		dc->VSSetShader(this->data_->GetVertexShader(base_and_outline), nullptr, 0);
		dc->PSSetShader(this->data_->GetPixelShader(base_and_outline), nullptr, 0);
		dc->IASetInputLayout(this->data_->GetInputLayout(input_layout));
		dc->PSSetSamplers(0, 1, this->data_->GetSamplerState(sampler_state));
		dc->PSSetSamplers(1, 1, this->data_->GetSamplerState(sampler_state_shadow));
		dc->PSSetShaderResources(0, 1, this->data_->GetTexture(texture));
		dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		this->GetComponent<DrawIndexed>()->Draw(this->graphics_, this->constant_buffer_, this->data_->GetVertexBuffers(this->tag_), this->data_->GetIndexBuffers(this->tag_), this->data_->GetIndexSize(this->tag_));
	}
	break;
	}
}

#include "..\entity\tree.h"

#include "..\graphics\directx11.h"
#include "..\framework\data.h"
#include "..\model\model_import.h"
#include "..\component\draw\draw_indexed_instanced.h"
#include "..\framework\window.h"
#include "..\framework\scene.h"
#include "..\framework\math.h"
#include "..\framework\collision_math.h"
#include "..\entity\mesh_field.h"
#include "..\component\camera\tps.h"

#include <random>

constexpr char* depth = "instanced_pcf_depth";
constexpr char* base_and_outline = "instanced_pcf_base";
constexpr InputLayoutType input_layout = InputLayoutType::SimpleInstance;
constexpr SamplerStateType sampler_state = SamplerStateType::Simple;
constexpr SamplerStateType sampler_state_shadow = SamplerStateType::Shadow;
constexpr Texture texture = Texture::T_Tree;
constexpr char* instance = "tree_i";
constexpr unsigned int instance_num = 30;
constexpr float scaling = 1.f;

using namespace DirectX;

Tree::Tree(Scene* parent) : Entity(parent)
{
	this->tag_ = "tree";

	auto model = ModelImport::ModelData(this->tag_);

	this->data_->CreateVertexShaderAndInputLayout(depth, input_layout);
	this->data_->CreatePixelShader(depth);
	this->data_->CreateVertexShaderAndInputLayout(base_and_outline, input_layout);
	this->data_->CreatePixelShader(base_and_outline);
	this->data_->CreateConstantBuffer(this->constant_buffer_, ConstantBufferType::SimpleShadowAndOutline);
	this->data_->CreateVertexBuffers(this->tag_, VertexBufferType::Model, model);
	this->data_->CreateIndexBuffers(this->tag_, model);
	this->data_->CreateSamplerState(sampler_state);
	this->data_->CreateSamplerState(sampler_state_shadow);
	this->data_->CreateTexture(texture);

	this->AddComponent<DrawIndexedInstanced>(model.meshes.size());

	CBufferSimpleShadowAndOutline cb;
	cb.world = XMMatrixIdentity();
	cb.texel_size = DirectX::XMFLOAT4(1.0f / (1024 * depth_texture_size), 1.0f / (1024 * depth_texture_size), 0, 0);
	this->data_->UpdateConstantBuffer(this->constant_buffer_, &cb);

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> rot_y(0, 359);
	VertexBufferInstance vbi[instance_num];
	this->vbi_data_.resize(instance_num);
	for (unsigned int num = 0; num < instance_num; ++num)
	{
		float y = DirectX::XMConvertToRadians(rot_y(mt));
		XMFLOAT3 pos(static_cast<float>(Math::HaltonSeq(num, 2) * (field_width - 10)) + 5, 0, static_cast<float>(Math::HaltonSeq(num, 3) * (field_height - 10)) + 5);
		vbi[num].color = DirectX::XMFLOAT4(1, 1, 1, 1);
		vbi[num].world = DirectX::XMMatrixScaling(scaling, scaling, scaling)
			* DirectX::XMMatrixRotationY(y)
			* DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);

		this->vbi_data_[num].rot_y = y;
		this->vbi_data_[num].position = pos;
	}
	this->data_->CreateVertexBufferInstanced(instance, vbi, instance_num);
}

Tree::~Tree()
{

}

void Tree::Update()
{
	XMMATRIX view;
	for (auto& entity : this->parent_->GetEntities())
	{
		if (entity->FindWithTag("player"))
		{
			if (entity->HasComponent<TPS>())
			{
				view = entity->GetComponent<TPS>()->GetView();
			}
			break;
		}
	}

	VertexBufferInstance vbi[instance_num];
	for (unsigned int num = 0; num < instance_num; ++num)
	{
		if (CollisionMath::ViewFrustum(this->vbi_data_[num].position, 5.f, view, 60, (float)init_width / init_height, 0.1f, 30))
		{
			vbi[num].color = XMFLOAT4(1, 1, 1, 1);
			vbi[num].world = XMMatrixScaling(scaling, scaling, scaling)
				* XMMatrixRotationY(this->vbi_data_[num].rot_y)
				* XMMatrixTranslation(this->vbi_data_[num].position.x, this->vbi_data_[num].position.y, this->vbi_data_[num].position.z);
		}
		else
		{
			vbi[num].world = XMMatrixScaling(0, 0, 0);
		}
	}
	this->data_->UpdateVertexBufferInstanced(instance, vbi);
}

void Tree::Draw(RenderingPass pass)
{
	const auto& dc = this->graphics_->device_context_;

	switch (pass)
	{
	case RenderingPass::Depth:
	{
		this->graphics_->SetRasterizerState(RasterizerState::CullNone);

		dc->VSSetShader(this->data_->GetVertexShader(depth), nullptr, 0);
		dc->PSSetShader(this->data_->GetPixelShader(depth), nullptr, 0);
		dc->IASetInputLayout(this->data_->GetInputLayout(input_layout));
		dc->PSSetSamplers(0, 1, this->data_->GetSamplerState(sampler_state));
		dc->PSSetSamplers(1, 1, this->data_->GetSamplerState(sampler_state_shadow));
		dc->PSSetShaderResources(0, 1, this->data_->GetTexture(texture));
		dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		this->GetComponent<DrawIndexedInstanced>()->Draw(this->graphics_, this->constant_buffer_, this->data_->GetVertexBuffers(this->tag_), this->data_->GetVertexBuffers(instance), this->data_->GetIndexBuffers(this->tag_), this->data_->GetIndexSize(this->tag_), instance_num);

		this->graphics_->SetRasterizerState(RasterizerState::Default);
	}
	break;

	case RenderingPass::Base:
	{
		this->graphics_->SetRasterizerState(RasterizerState::CullNone);
		this->graphics_->SetBlendState(BlendState::AlphaToCoverage);

		dc->VSSetShader(this->data_->GetVertexShader(base_and_outline), nullptr, 0);
		dc->PSSetShader(this->data_->GetPixelShader(base_and_outline), nullptr, 0);
		dc->IASetInputLayout(this->data_->GetInputLayout(input_layout));
		dc->PSSetSamplers(0, 1, this->data_->GetSamplerState(sampler_state));
		dc->PSSetSamplers(1, 1, this->data_->GetSamplerState(sampler_state_shadow));
		dc->PSSetShaderResources(0, 1, this->data_->GetTexture(texture));
		dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		this->GetComponent<DrawIndexedInstanced>()->Draw(this->graphics_, this->constant_buffer_, this->data_->GetVertexBuffers(this->tag_), this->data_->GetVertexBuffers(instance), this->data_->GetIndexBuffers(this->tag_), this->data_->GetIndexSize(this->tag_), instance_num);

		this->graphics_->SetRasterizerState(RasterizerState::Default);
		this->graphics_->SetBlendState(BlendState::Default);
	}
	break;
	}
}
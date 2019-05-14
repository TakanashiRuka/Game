#include "..\entity\grass.h"

#include "..\graphics\directx11.h"
#include "..\framework\data.h"
#include "..\model\model_import.h"
#include "..\component\draw\draw_indexed_instanced.h"
#include "..\framework\window.h"
#include "..\framework\scene.h"
#include "..\framework\math.h"
#include "..\framework\collision_math.h"
#include "..\entity\mesh_field.h"
#include "..\entity\title_camera.h"
#include "..\component\camera\tps.h"

#include <random>

constexpr char* hlsl = "instanced_simple";
constexpr InputLayoutType input_layout = InputLayoutType::SimpleInstance;
constexpr SamplerStateType sampler_state = SamplerStateType::Simple;
constexpr Texture texture = Texture::T_Grass;
constexpr char* instance = "grass_i";
constexpr unsigned int instance_num = field_width * field_height / 10;
constexpr float scaling = 0.011f;

using namespace DirectX;

Grass::Grass(Scene* parent) : Entity(parent)
{
	this->tag_ = "grass";

	auto model = ModelImport::ModelData(this->tag_);

	this->data_->CreateVertexShaderAndInputLayout(hlsl, input_layout);
	this->data_->CreatePixelShader(hlsl);
	this->data_->CreateVertexBuffers(this->tag_, VertexBufferType::Model, model);
	this->data_->CreateIndexBuffers(this->tag_, model);
	this->data_->CreateSamplerState(sampler_state);
	this->data_->CreateTexture(texture);

	this->AddComponent<DrawIndexedInstanced>(model.meshes.size());

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> rot_y(0, 359);
	float pos_y = 0.06f;
	VertexBufferInstance vbi[instance_num];
	this->vbi_data_.resize(instance_num);
	for (unsigned int num = 0; num < instance_num; ++num)
	{
		float y = XMConvertToRadians(rot_y(mt));
		XMFLOAT3 pos(static_cast<float>(Math::HaltonSeq(num, 2) * (field_width - 6)) + 3, pos_y, static_cast<float>(Math::HaltonSeq(num, 5) * (field_height - 6)) + 3);
		vbi[num].color = XMFLOAT4(1, 1, 1, 1);
		vbi[num].world = XMMatrixScaling(scaling, scaling, scaling)
			* XMMatrixRotationY(y)
			* XMMatrixTranslation(pos.x, pos.y, pos.z);

		this->vbi_data_[num].rot_y = y;
		this->vbi_data_[num].position = pos;
	}
	this->data_->CreateVertexBufferInstanced(instance, vbi, instance_num);
}

Grass::~Grass()
{

}

void Grass::Update()
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
		else if (entity->FindWithTag("title camera"))
		{
			view = static_cast<TitleCamera*>(entity)->GetView();
			break;
		}
	}

	VertexBufferInstance vbi[instance_num];
	for (unsigned int num = 0; num < instance_num; ++num)
	{
		if (CollisionMath::ViewFrustum(this->vbi_data_[num].position, 1.f, view, 60, (float)init_width / init_height, 0.1f, 30))
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

void Grass::Draw(RenderingPass pass)
{
	const auto& dc = this->graphics_->device_context_;

	switch (pass)
	{
	case RenderingPass::Base:
	{
		this->graphics_->SetBlendState(BlendState::AlphaToCoverage);

		dc->VSSetShader(this->data_->GetVertexShader(hlsl), nullptr, 0);
		dc->PSSetShader(this->data_->GetPixelShader(hlsl), nullptr, 0);
		dc->IASetInputLayout(this->data_->GetInputLayout(input_layout));
		dc->PSSetSamplers(0, 1, this->data_->GetSamplerState(sampler_state));
		dc->PSSetShaderResources(0, 1, this->data_->GetTexture(texture));
		dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		this->GetComponent<DrawIndexedInstanced>()->Draw(this->graphics_, this->constant_buffer_, this->data_->GetVertexBuffers(this->tag_), this->data_->GetVertexBuffers(instance), this->data_->GetIndexBuffers(this->tag_), this->data_->GetIndexSize(this->tag_), instance_num);

		this->graphics_->SetBlendState(BlendState::Default);
	}
	break;
	}
}
#include "..\entity\rock.h"

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

constexpr char* hlsl = "instanced_simple";
constexpr InputLayoutType input_layout = InputLayoutType::SimpleInstance;
constexpr SamplerStateType sampler_state = SamplerStateType::Simple;
constexpr Texture texture = Texture::T_Rock;
constexpr char* instance = "rock_i";
constexpr unsigned int instance_num = 200;
constexpr float scaling = 1;

using namespace DirectX;

Rock::Rock(Scene* parent) : Entity(parent)
{
	this->tag_ = "rock";

	auto model = ModelImport::ModelData(this->tag_);

	this->data_->CreateVertexShaderAndInputLayout(hlsl, input_layout);
	this->data_->CreatePixelShader(hlsl);
	this->data_->CreateVertexBuffers(this->tag_, VertexBufferType::Model, model);
	this->data_->CreateIndexBuffers(this->tag_, model);
	this->data_->CreateSamplerState(sampler_state);
	this->data_->CreateTexture(texture);

	this->AddComponent<DrawIndexedInstanced>(model.meshes.size());

	this->vbi_data_.resize(instance_num);
	VertexBufferInstance vbi[instance_num];
	for (unsigned int num = 0; num < instance_num; ++num)
	{
		vbi[num].color = DirectX::XMFLOAT4(1, 1, 1, 1);
		vbi[num].world = DirectX::XMMatrixScaling(0, 0, 0);
	}
	float y = 0.5f;
	unsigned int cnt = 0;
	for (float x = 0; x < static_cast<float>(field_width); x += 2, ++cnt)
	{
		vbi[cnt].world = DirectX::XMMatrixScaling(scaling, scaling, scaling) * XMMatrixRotationY(XMConvertToRadians(0)) * DirectX::XMMatrixTranslation(x, y, static_cast<float>(field_height));
		this->vbi_data_[cnt].position = XMFLOAT3(x, y, static_cast<float>(field_height));
		this->vbi_data_[cnt].rot_y = XMConvertToRadians(0);
	}
	for (float z = static_cast<float>(field_height); z > 0; z -= 2, ++cnt)
	{
		vbi[cnt].world = DirectX::XMMatrixScaling(scaling, scaling, scaling) * XMMatrixRotationY(XMConvertToRadians(90)) * DirectX::XMMatrixTranslation(static_cast<float>(field_width), y, z);
		this->vbi_data_[cnt].position = XMFLOAT3(static_cast<float>(field_width), y, z);
		this->vbi_data_[cnt].rot_y = XMConvertToRadians(90);
	}
	for (float x = static_cast<float>(field_width); x > 0; x -= 2, ++cnt)
	{
		vbi[cnt].world = DirectX::XMMatrixScaling(scaling, scaling, scaling) * XMMatrixRotationY(XMConvertToRadians(0)) * DirectX::XMMatrixTranslation(x, y, 0);
		this->vbi_data_[cnt].position = XMFLOAT3(x, y, 0);
		this->vbi_data_[cnt].rot_y = XMConvertToRadians(0);
	}
	for (float z = 0; z < static_cast<float>(field_height); z += 2, ++cnt)
	{
		vbi[cnt].world = DirectX::XMMatrixScaling(scaling, scaling, scaling) * XMMatrixRotationY(XMConvertToRadians(90)) * DirectX::XMMatrixTranslation(0, y, z);
		this->vbi_data_[cnt].position = XMFLOAT3(0, y, z);
		this->vbi_data_[cnt].rot_y = XMConvertToRadians(90);
	}
	this->data_->CreateVertexBufferInstanced(instance, vbi, instance_num);
}

Rock::~Rock()
{

}

void Rock::Update()
{
	//XMMATRIX view;
	//for (auto& entity : this->parent_->GetEntities())
	//{
	//	if (entity->FindWithTag("player"))
	//	{
	//		if (entity->HasComponent<TPS>())
	//		{
	//			view = entity->GetComponent<TPS>()->GetView();
	//		}
	//		break;
	//	}
	//}

	//VertexBufferInstance vbi[instance_num];
	//for (unsigned int num = 0; num < instance_num; ++num)
	//{
	//	if (CollisionMath::ViewFrustum(this->vbi_data_[num].position, 1.f, view, 60, (float)init_width / init_height, 0.1f, 30))
	//	{
	//		vbi[num].color = XMFLOAT4(1, 1, 1, 1);
	//		vbi[num].world = XMMatrixScaling(scaling, scaling, scaling)
	//			* XMMatrixRotationY(this->vbi_data_[num].rot_y)
	//			* XMMatrixTranslation(this->vbi_data_[num].position.x, this->vbi_data_[num].position.y, this->vbi_data_[num].position.z);
	//	}
	//	else
	//	{
	//		vbi[num].world = XMMatrixScaling(0, 0, 0);
	//	}
	//}
	//this->data_->UpdateVertexBufferInstanced(instance, vbi);
}

void Rock::Draw(RenderingPass pass)
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
		this->GetComponent<DrawIndexedInstanced>()->Draw(this->graphics_, this->constant_buffer_, this->data_->GetVertexBuffers(this->tag_), this->data_->GetVertexBuffers(instance), this->data_->GetIndexBuffers(this->tag_), this->data_->GetIndexSize(this->tag_), field_width + field_height);
	}
	break;
	}
}
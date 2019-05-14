#include "..\entity\player.h"

#include "..\graphics\directx11.h"
#include "..\framework\data.h"
#include "..\model\model_import.h"
#include "..\component\draw\draw_indexed.h"
#include "..\component\movement.h"
#include "..\component\camera\tps.h"
#include "..\framework\game.h"
#include "..\framework\collision_math.h"
#include "..\entity\light.h"
#include "..\component\collider\mesh_collider.h"

constexpr char* sma_depth = "skin_mesh_animation_pcf_depth";
constexpr char* sma_base = "skin_mesh_animation_pcf_base";
constexpr InputLayoutType input_layout = InputLayoutType::SkinMeshAnimation;
constexpr SamplerStateType sampler_state = SamplerStateType::Simple;
constexpr SamplerStateType sampler_state_shadow = SamplerStateType::Shadow;
constexpr Texture texture = Texture::T_Player;
constexpr float scale = 0.005f;

using namespace DirectX;

std::string old_key;

Player::Player(Scene* parent) : Entity(parent)
{
	this->tag_ = "player";

	auto model = ModelImport::ModelData(this->tag_);

	this->data_->CreateVertexShaderAndInputLayout(sma_depth, input_layout);
	this->data_->CreatePixelShader(sma_depth);
	this->data_->CreateVertexShaderAndInputLayout(sma_base, input_layout);
	this->data_->CreatePixelShader(sma_base);
	this->data_->CreateConstantBuffers(this->constant_buffers_, ConstantBufferType::AnimationShadow, model.meshes.size());
	this->data_->CreateVertexBuffers(this->tag_, VertexBufferType::Model, model);
	this->data_->CreateIndexBuffers(this->tag_, model);
	this->data_->CreateSamplerState(sampler_state);
	this->data_->CreateTexture(texture);
	this->data_->CreateAnimation(this->tag_, model);

	this->AddComponent<DrawIndexed>(model.meshes.size());
	this->AddComponent<Movement>(0.05f, 10.0f);
	this->AddComponent<TPS>(this->data_, 2.0f, 2.0f, 0.1f);
}

Player::~Player()
{

}

void Player::Update()
{
	const auto& movement = this->GetComponent<Movement>();
	const auto& tps = this->GetComponent<TPS>();

	movement->Update();
	auto& pos = movement->GetPos();

	Entity* light = nullptr;
	for (auto& entity : this->parent_->GetEntities())
	{
		if (entity->FindWithTag("mesh_field"))
		{
			if (entity->HasComponent<MeshCollider>())
			{
				pos.y = CollisionMath::GetFieldY(entity->GetComponent<MeshCollider>()->GetMeshes(), pos);
			}
		}

		if (entity->FindWithTag("light"))
			light = entity;
	}
	const auto& rot_y = movement->GetRotY();
	tps->SetPos(pos);
	tps->Update(this->graphics_, this->data_);
	if (light)
		static_cast<Light*>(light)->SetLightPos(pos);

	CBufferAnimationShadow cb;
	cb.world = XMMatrixScaling(scale, scale, scale) * XMMatrixRotationY(-XMConvertToRadians(180) + rot_y) * XMMatrixTranslation(pos.x, pos.y, pos.z);
	cb.texel_size = XMFLOAT4(1.0f / (1024 * depth_texture_size), 1.0f / (1024 * depth_texture_size), 0, 0);
	this->animation_running_time_ += Game::GetInstance()->GetRunningTime();
	std::string key;
	if (!movement->Move())
	{
		key = "UnarmedIdle";
	}
	else
	{
		key = "StandardRun";
	}
	if (old_key != key)
	{
		this->animation_running_time_ = 0;
		old_key = key;
	}
	for (unsigned int s = 0; s < this->constant_buffers_.size(); ++s)
	{
		this->data_->UpdateAnimation(this->tag_, key, this->animation_running_time_, cb.bones, s);
		this->data_->UpdateConstantBuffer(this->constant_buffers_[s], &cb);
	}
}

void Player::Draw(RenderingPass pass)
{
	const auto& dc = this->graphics_->device_context_;

	switch (pass)
	{
	case RenderingPass::Depth:
	{
		dc->VSSetShader(this->data_->GetVertexShader(sma_depth), nullptr, 0);
		dc->PSSetShader(this->data_->GetPixelShader(sma_depth), nullptr, 0);
		dc->IASetInputLayout(this->data_->GetInputLayout(input_layout));
		dc->PSSetSamplers(1, 1, this->data_->GetSamplerState(sampler_state_shadow));
		dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		this->GetComponent<DrawIndexed>()->DrawAnimation(this->graphics_, this->constant_buffers_, this->data_->GetVertexBuffers(this->tag_), this->data_->GetIndexBuffers(this->tag_), this->data_->GetIndexSize(this->tag_));
	}
	break;

	case RenderingPass::Base:
	{
		dc->VSSetShader(this->data_->GetVertexShader(sma_base), nullptr, 0);
		dc->PSSetShader(this->data_->GetPixelShader(sma_base), nullptr, 0);
		dc->IASetInputLayout(this->data_->GetInputLayout(input_layout));
		dc->PSSetSamplers(0, 1, this->data_->GetSamplerState(sampler_state));
		dc->PSSetSamplers(1, 1, this->data_->GetSamplerState(sampler_state_shadow));
		dc->PSSetShaderResources(0, 1, this->data_->GetTexture(texture));
		dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		this->GetComponent<DrawIndexed>()->DrawAnimation(this->graphics_, this->constant_buffers_, this->data_->GetVertexBuffers(this->tag_), this->data_->GetIndexBuffers(this->tag_), this->data_->GetIndexSize(this->tag_));
	}
	break;

	case RenderingPass::Debug:
	{
		this->GetComponent<TPS>()->Draw();
		Debug::DrawObject(this->tag_, this->GetComponent<Movement>()->GetPos());
		XMFLOAT3 a((float)this->animation_running_time_, this->GetComponent<Movement>()->GetSpeed(), 0);
		Debug::DrawObject("player_sub", a);
		Debug::DrawInput();
	}
	break;
	}
}
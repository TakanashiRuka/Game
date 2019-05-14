#include "..\entity\target.h"

#include "..\graphics\directx11.h"
#include "..\framework\data.h"
#include "..\model\model_import.h"
#include "..\component\draw\draw_indexed.h"
#include "..\framework\scene.h"
#include "..\framework\collision_math.h"
#include "..\component\camera\tps.h"
#include "..\component\movement.h"
#include "..\component\collider\sphere_collider.h"
#include "..\entity\fruit.h"
#include "..\entity\score.h"
#include "..\entity\theme.h"
#include "..\entity\combo.h"
#include "..\entity\speed_up.h"
#include "..\entity\score_up.h"
#include "..\entity\add_fruit.h"
#include "..\entity\bill_board_animation.h"
#include "..\entity\color.h"
#include "..\sound\x_audio2.h"
#include "..\input\keyboard.h"
#include "..\input\x_input.h"
#include "..\framework\debug.h"

constexpr char* hlsl = "simple_bill_board";
constexpr InputLayoutType input_layout = InputLayoutType::Simple;
constexpr SamplerStateType sampler_state = SamplerStateType::Simple;
constexpr Texture texture = Texture::T_Target;
constexpr unsigned int texture_animation_max = 58;

Target::Target(Scene* parent) : Entity(parent)
{
	this->tag_ = "target";

	auto model = ModelImport::BillBoardData(1, 1, 0, 0, 0, 0);

	this->data_->CreateVertexShaderAndInputLayout(hlsl, input_layout);
	this->data_->CreatePixelShader(hlsl);
	this->data_->CreateConstantBuffer(this->constant_buffer_, ConstantBufferType::Simple);
	this->data_->CreateVertexBuffers(this->tag_, VertexBufferType::UI, model);
	this->data_->CreateIndexBuffers(this->tag_, model);
	this->data_->CreateSamplerState(sampler_state);
	this->data_->CreateTexture(texture);

	this->AddComponent<DrawIndexed>(model.meshes.size());

	XAudio2::AddSourceVoice(this->tag_, SoundType::SE);

	this->uv_x_ = 0;
	this->uv_w_ = 1.0f / texture_animation_max;
}

Target::~Target()
{

}

void Target::Update()
{
	if (this->texture_animation_ < texture_animation_max)
	{
		this->uv_x_ += this->uv_w_;
		++this->texture_animation_;
	}
	else
	{
		this->texture_animation_ = 1;
		this->uv_x_ = 0;
	}

	CBufferSimple cb;
	DirectX::XMFLOAT3 position = { 0, FLT_MAX, 0 };
	DirectX::XMFLOAT3 player_pos;
	float dist = FLT_MAX;
	Component* movement = nullptr;
	Entity* fruit = nullptr;
	Entity* score = nullptr;
	Entity* time_up = nullptr;
	Entity* theme = nullptr;
	Entity* combo = nullptr;
	for (auto& entity : this->parent_->GetEntities())
	{
		if (entity->FindWithTag("player"))
		{
			if (entity->HasComponent<Movement>())
			{
				player_pos = entity->GetComponent<Movement>()->GetPos();
				movement = entity->GetComponent<Movement>();
			}
			if (entity->HasComponent<TPS>())
				cb.world = DirectX::XMMatrixInverse(nullptr, entity->GetComponent<TPS>()->GetView());
		}

		if (entity->FindWithTag("fruit"))
		{
			if (entity->HasComponent<SphereCollider>())
			{
				const auto& sc = entity->GetComponent<SphereCollider>();
				auto start = player_pos;
				auto end = start;
				end.y += 1;
				float radius = 0.5f;
				if (CollisionMath::SphereAndCapsule(sc->GetPosition(), sc->GetRadius(), start, end, radius))
				{
					float c_dist = std::abs(std::hypot(sc->GetPosition().x - start.x, sc->GetPosition().z - start.z));
					if (dist > c_dist)
					{
						dist = c_dist;
						position = sc->GetPosition();
						fruit = entity;
					}
				}
			}
		}

		if (entity->FindWithTag("score"))
			score = entity;

		if (entity->FindWithTag("time up"))
			time_up = entity;

		if (entity->FindWithTag("theme"))
			theme = entity;

		if (entity->FindWithTag("combo"))
			combo = entity;
	}

	if ((this->old_pos_.x != position.x) || (this->old_pos_.y != position.y) || (this->old_pos_.z != position.z))
	{
		this->texture_animation_ = 1;
		this->uv_x_ = 0;
		this->old_pos_ = position;
		if (!time_up && (position.y != FLT_MAX))
			XAudio2::StartSourceVoice(this->tag_);
	}
	cb.world.r[3].m128_f32[0] = cb.world.r[3].m128_f32[1] = cb.world.r[3].m128_f32[2] = 0;
	cb.world *= DirectX::XMMatrixTranslation(position.x, position.y + 0.1f, position.z);
	this->data_->UpdateConstantBuffer(this->constant_buffer_, &cb);

	if (this->start_ && KeyBoard::Trigger('E') || XInput::Trigger(XINPUT_GAMEPAD_B))
	{
		if (fruit && !time_up)
		{
			this->parent_->AddEntity<BillBoardAnimation>(BillBoardAnimationType::Blast, position, static_cast<Fruit*>(fruit)->GetOutLineColor());
			this->parent_->AddEntity<Fruit>();
			fruit->Remove();
			if (score && fruit && theme && (static_cast<Fruit*>(fruit)->GetUseTexture() == static_cast<Theme*>(theme)->GetThemeTexture()))
			{
				static_cast<Combo*>(combo)->AddCombo();
				if (static_cast<Combo*>(combo)->GetCombo() % 2 == 0)
					this->parent_->AddEntity<SpeedUp>();
				static_cast<Movement*>(movement)->SetSpeed(1 + static_cast<Combo*>(combo)->GetCombo() / 2 * 0.05f);
				if (static_cast<Combo*>(combo)->GetCombo() % 3 == 0)
					this->parent_->AddEntity<ScoreUp>();
				static_cast<Score*>(score)->AddScore(10 + 10 * (static_cast<Combo*>(combo)->GetCombo() / 3));
				if (static_cast<Combo*>(combo)->GetCombo() % 5 == 0)
				{
					this->parent_->AddEntity<AddFruit>();
					for (unsigned int num = 0; num < 10; ++num)
						this->parent_->AddEntity<Fruit>();
				}
				static_cast<Theme*>(theme)->ChangeThemeTexture();
			}
			else
			{
				static_cast<Combo*>(combo)->ResetCombo();
				static_cast<Movement*>(movement)->SetSpeed(1);
				static_cast<Score*>(score)->AddScore(10);
			}
		}
	}

	if (KeyBoard::Trigger(VK_RETURN) || XInput::Trigger(XINPUT_GAMEPAD_B))
		this->start_ = true;
}

void Target::Draw(RenderingPass pass)
{
	const auto& dc = this->graphics_->device_context_;

	switch (pass)
	{
	case RenderingPass::UnableDepthStencil:
	{
		this->graphics_->SetBlendState(BlendState::AlphaToCoverage);

		auto model = ModelImport::BillBoardData(0.3f, 0.3f, this->uv_x_, 0, this->uv_w_, 1);
		this->data_->UpdateVertexBuffers(this->tag_, model);

		dc->VSSetShader(this->data_->GetVertexShader(hlsl), nullptr, 0);
		dc->PSSetShader(this->data_->GetPixelShader(hlsl), nullptr, 0);
		dc->IASetInputLayout(this->data_->GetInputLayout(input_layout));
		dc->PSSetSamplers(0, 1, this->data_->GetSamplerState(sampler_state));
		dc->PSSetShaderResources(0, 1, this->data_->GetTexture(texture));
		dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		this->GetComponent<DrawIndexed>()->Draw(this->graphics_, this->constant_buffer_, this->data_->GetVertexBuffers(this->tag_), this->data_->GetIndexBuffers(this->tag_), this->data_->GetIndexSize(this->tag_));

		this->graphics_->SetBlendState(BlendState::Default);
	}
	break;

	case RenderingPass::Debug:
	{
		DirectX::XMFLOAT3 pos(this->uv_x_, this->uv_w_, static_cast<float>(this->texture_animation_));
		Debug::DrawObject(this->tag_, pos);
	}
	break;
	}
}
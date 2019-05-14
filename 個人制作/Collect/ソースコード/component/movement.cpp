#include "..\component\movement.h"

#include "..\framework\entity.h"
#include "..\component\camera\tps.h"
#include "..\entity\mesh_field.h"
#include "..\framework\debug.h"
#include "..\input\keyboard.h"
#include "..\input\x_input.h"

constexpr float pos_max = static_cast<float>(field_width) / 2 - 1.6f;

using namespace DirectX;

Movement::Movement(Entity* parent, float move, float degree) : Component(parent), move_(move), degree_(degree)
{
	this->old_pos_ = this->pos_;
}

Movement::~Movement()
{

}

void Movement::Update()
{
	this->old_pos_ = this->pos_;
	auto rot_y = this->parent_->GetComponent<TPS>()->GetRotY();

	float move_x = 0;
	float move_z = 0;
	if (KeyBoard::Press('W') || XInput::ThumbLYUp())
	{
		rot_y += XMConvertToRadians((KeyBoard::Press('A') - KeyBoard::Press('D') + XInput::ThumbLXLeft() - XInput::ThumbLXRight()) * 45.0f);
		move_x = this->move_ * -sinf(rot_y);
		move_z = this->move_ * cosf(rot_y);
	}
	else if (KeyBoard::Press('S') || XInput::ThumbLYDown())
	{
		rot_y -= XMConvertToRadians((KeyBoard::Press('A') - KeyBoard::Press('D') + XInput::ThumbLXLeft() - XInput::ThumbLXRight()) * 45.0f);
		move_x = -this->move_ * -sinf(rot_y);
		move_z = -this->move_ * cosf(rot_y);
	}
	else if (KeyBoard::Press('A') || XInput::ThumbLXLeft())
	{
		rot_y += XMConvertToRadians(90);
		move_x = this->move_ * -sinf(rot_y);
		move_z = this->move_ * cosf(rot_y);
	}
	else if (KeyBoard::Press('D') || XInput::ThumbLXRight())
	{
		rot_y -= XMConvertToRadians(90);
		move_x = this->move_ * -sinf(rot_y);
		move_z = this->move_ * cosf(rot_y);
	}

	if (KeyBoard::Press('W') || KeyBoard::Press('S') || KeyBoard::Press('A') || KeyBoard::Press('D') || XInput::ThumbLYUp() || XInput::ThumbLYDown() || XInput::ThumbLXLeft() || XInput::ThumbLXRight())
	{
		float radian = XMConvertToRadians(this->degree_);
		float cross = 0;
		XMStoreFloat(&cross, XMVector2Cross(XMVectorSet(move_x, move_z, 0, 0), XMVectorSet(-sinf(this->rot_y_), cosf(this->rot_y_), 0, 0)));
		float y = std::atan2(move_x, move_z);
		this->rot_y_ += radian * ((cross > 0) - (cross < 0));
		this->rot_y_ = y;
	}

	this->pos_.x += move_x * this->speed_;
	this->pos_.z += move_z * this->speed_;

	if (this->pos_.x < field_width / 2 - pos_max)
		this->pos_.x = field_width / 2 - pos_max;
	else if (this->pos_.x > field_width / 2 + pos_max)
		this->pos_.x = field_width / 2 + pos_max;
	if (this->pos_.z < field_height / 2 - pos_max)
		this->pos_.z = field_height / 2 - pos_max;
	else if (this->pos_.z > field_height / 2 + pos_max)
		this->pos_.z = field_height / 2 + pos_max;
}

void Movement::SetPosY(float pos_y)
{
	this->pos_.y = pos_y;
}

void Movement::SetSpeed(float speed)
{
	this->speed_ = speed;
}

DirectX::XMFLOAT3& Movement::GetPos()
{
	return this->pos_;
}

float Movement::GetSpeed()
{
	return this->speed_;
}

float const Movement::GetRotY()
{
	return this->rot_y_;
}

bool Movement::Move()
{
	bool move = false;
	if ((this->old_pos_.x != this->pos_.x) || (this->old_pos_.y != this->pos_.y) || (this->old_pos_.z != this->pos_.z))
		move = true;

	return move;
}
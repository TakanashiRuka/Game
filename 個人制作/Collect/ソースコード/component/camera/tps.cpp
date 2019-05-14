#include "..\..\component\camera\tps.h"

#include "..\..\framework\window.h"
#include "..\..\graphics\directx11.h"
#include "..\..\framework\data.h"
#include "..\..\framework\debug.h"
#include "..\..\framework\utility.h"
#include "..\..\input\keyboard.h"
#include "..\..\input\x_input.h"

using namespace DirectX;

constexpr float fov = 60.0f;
constexpr float aspect = (float)init_width / init_height;
constexpr float z_near = 0.1f;
constexpr float z_far = 1000.0f;

TPS::TPS(Entity* parent, Data* data, float radian_x, float radian_y, float move) : Component(parent), radian_x_(radian_x), radian_y_(radian_y), move_(move)
{
	data->CreateConstantBuffer(this->constant_buffer_, ConstantBufferType::Camera);
}

TPS::~TPS()
{
	SafeRelease(this->constant_buffer_);
}

void TPS::SetPos(DirectX::XMFLOAT3 pos)
{
	if (!this->free_)
		this->pos_ = pos;
}

DirectX::XMMATRIX const TPS::GetView()
{
	return XMMatrixTranslation(-this->pos_.x, -this->pos_.y - 0.5f, -this->pos_.z) * XMMatrixRotationY(this->rot_.y) * XMMatrixRotationX(this->rot_.x) * XMMatrixLookAtLH(XMLoadFloat3(&this->eye_), XMLoadFloat3(&this->at_), XMLoadFloat3(&this->up_));
}

float const TPS::GetRotX()
{
	return this->rot_.x;
}

float const TPS::GetRotY()
{
	return this->rot_.y;
}

void TPS::Update(Graphics* graphics, Data* data)
{
	float add_x = XMConvertToRadians((KeyBoard::Press(VK_UP) - KeyBoard::Press(VK_DOWN) + XInput::ThumbRYUp() - XInput::ThumbRYDown()) * this->radian_x_);
	this->rot_.x += add_x;
	if ((XMConvertToDegrees(this->rot_.x) < -70) || (XMConvertToDegrees(this->rot_.x) > 10)) this->rot_.x -= add_x;
	this->rot_.y += XMConvertToRadians((KeyBoard::Press(VK_LEFT) - KeyBoard::Press(VK_RIGHT) + XInput::ThumbRXLeft() - XInput::ThumbRXRight()) * this->radian_y_);
	if ((XMConvertToDegrees(this->rot_.y) >= 360) || (XMConvertToDegrees(this->rot_.y) <= -360)) this->rot_.y = 0;

	CBufferCamera cb;
	cb.view = XMMatrixTranslation(-this->pos_.x, -this->pos_.y - 0.5f, -this->pos_.z) * XMMatrixRotationY(this->rot_.y) * XMMatrixRotationX(this->rot_.x) * XMMatrixLookAtLH(XMLoadFloat3(&this->eye_), XMLoadFloat3(&this->at_), XMLoadFloat3(&this->up_));
	cb.projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(fov), aspect, z_near, z_far);
	cb.view_inverse = XMMatrixInverse(nullptr, cb.view);
	cb.eye = this->eye_;
	data->UpdateConstantBuffer(this->constant_buffer_, &cb);

	graphics->device_context_->VSSetConstantBuffers(3, 1, &this->constant_buffer_);
	graphics->device_context_->PSSetConstantBuffers(3, 1, &this->constant_buffer_);
}

void TPS::Draw()
{
	Debug::DrawCameraTPS(this->eye_, this->at_, this->up_, this->pos_, this->rot_, this->free_);
}
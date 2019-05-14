#include "..\entity\title_camera.h"

#include "..\framework\window.h"
#include "..\graphics\directx11.h"
#include "..\framework\data.h"
#include "..\framework\scene.h"
#include "..\entity\light.h"
#include "..\entity\mesh_field.h"

constexpr float fov = 60.0f;
constexpr float aspect = (float)init_width / init_height;
constexpr float z_near = 0.1f;
constexpr float z_far = 1000.0f;

using namespace DirectX;

TitleCamera::TitleCamera(Scene* parent) : Entity(parent)
{
	this->tag_ = "title camera";

	this->data_->CreateConstantBuffer(this->constant_buffer_, ConstantBufferType::Camera);

	this->rot_x_ = XMConvertToRadians(-40);

	for (auto& entity : this->parent_->GetEntities())
		if (entity->FindWithTag("light"))
			static_cast<Light*>(entity)->SetLightPos(DirectX::XMFLOAT3(static_cast<float>(field_width) / 2, 0, static_cast<float>(field_height) / 2));
}

TitleCamera::~TitleCamera()
{

}

void TitleCamera::Update()
{
	this->rot_y_ += DirectX::XMConvertToRadians(0.2f);
	if (this->rot_y_ >= 360)
		this->rot_y_ = 0;

	CBufferCamera cb;
	cb.view = XMMatrixTranslation(-static_cast<float>(field_width) / 2, -0.5f, -static_cast<float>(field_height) / 2)
		* XMMatrixRotationY(this->rot_y_) * XMMatrixRotationX(this->rot_x_)
		* XMMatrixLookAtLH(XMVectorSet(0, 0, -1.4f, 0), XMVectorSet(0, 0, 0, 0), XMVectorSet(0, 1, 0, 0));
	cb.projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(fov), aspect, z_near, z_far);
	cb.eye = XMFLOAT3(0, 0, 1.5f);
	this->data_->UpdateConstantBuffer(this->constant_buffer_, &cb);

	this->graphics_->device_context_->VSSetConstantBuffers(3, 1, &this->constant_buffer_);
	this->graphics_->device_context_->PSSetConstantBuffers(3, 1, &this->constant_buffer_);
}

float TitleCamera::GetRotX()
{
	return this->rot_x_;
}

float TitleCamera::GetRotY()
{
	return this->rot_y_;
}

DirectX::XMMATRIX TitleCamera::GetView()
{
	return XMMatrixTranslation(-static_cast<float>(field_width) / 2, -0.5f, -static_cast<float>(field_height) / 2)
		* XMMatrixRotationY(this->rot_y_) * XMMatrixRotationX(this->rot_x_)
		* XMMatrixLookAtLH(XMVectorSet(0, 0, -1.4f, 0), XMVectorSet(0, 0, 0, 0), XMVectorSet(0, 1, 0, 0));
}

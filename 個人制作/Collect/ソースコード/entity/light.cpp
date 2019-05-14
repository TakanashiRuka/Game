#include "..\entity\light.h"

#include "..\graphics\directx11.h"
#include "..\framework\data.h"
#include "..\framework\debug.h"

constexpr float view_width = 10;
constexpr float view_height = 10;
constexpr float near_z = 0.1f;
constexpr float far_z = 17.3f;

using namespace DirectX;

Light::Light(Scene* parent) : Entity(parent)
{
	this->tag_ = "light";

	this->data_->CreateConstantBuffer(this->constant_buffer_, ConstantBufferType::Light);

	CBufferLight cb;
	cb.light_view = XMMatrixLookAtLH(XMLoadFloat4(&this->light_), XMVectorSet(0, 0, 0, 0), XMVectorSet(0, 1, 0, 0));
	cb.light_projection = XMMatrixOrthographicLH(view_width, view_height, near_z, far_z);
	cb.light = this->light_;
	this->data_->UpdateConstantBuffer(this->constant_buffer_, &cb);

	this->graphics_->device_context_->VSSetConstantBuffers(2, 1, &this->constant_buffer_);
	this->graphics_->device_context_->PSSetConstantBuffers(2, 1, &this->constant_buffer_);
}

Light::~Light()
{

}

void Light::Draw(RenderingPass pass)
{
	switch (pass)
	{
	case RenderingPass::Debug:
	{
		Debug::DrawLight(this->light_, this->ambient_, this->diffuse_, this->specular_);
	}
	break;
	}
}

void Light::SetLightPos(DirectX::XMFLOAT3 pos)
{
	CBufferLight cb;
	cb.light_view = XMMatrixLookAtLH(XMVectorSet(this->light_.x + pos.x, this->light_.y, this->light_.z + pos.z, this->light_.w), XMVectorSet(pos.x, 0, pos.z, 0), XMVectorSet(0, 1, 0, 0));
	cb.light_projection = XMMatrixOrthographicLH(view_width, view_height, near_z, far_z);
	cb.light = this->light_;
	this->data_->UpdateConstantBuffer(this->constant_buffer_, &cb);

	this->graphics_->device_context_->VSSetConstantBuffers(2, 1, &this->constant_buffer_);
	this->graphics_->device_context_->PSSetConstantBuffers(2, 1, &this->constant_buffer_);
}
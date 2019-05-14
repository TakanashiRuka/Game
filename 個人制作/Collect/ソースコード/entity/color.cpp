#include "..\entity\color.h"

#include "..\graphics\directx11.h"
#include "..\framework\data.h"

Color::Color(Scene* parent) : Entity(parent)
{
	this->tag_ = "color";

	this->data_->CreateConstantBuffer(this->constant_buffer_, ConstantBufferType::Color);

	CBufferColor cb;
	this->data_->UpdateConstantBuffer(this->constant_buffer_, &cb);

	this->graphics_->device_context_->VSSetConstantBuffers(1, 1, &this->constant_buffer_);
	this->graphics_->device_context_->PSSetConstantBuffers(1, 1, &this->constant_buffer_);
}

Color::~Color()
{

}

void Color::SetColor(DirectX::XMFLOAT4 color)
{
	CBufferColor cb;
	cb.color = color;
	this->data_->UpdateConstantBuffer(this->constant_buffer_, &cb);

	this->graphics_->device_context_->VSSetConstantBuffers(1, 1, &this->constant_buffer_);
	this->graphics_->device_context_->PSSetConstantBuffers(1, 1, &this->constant_buffer_);
}
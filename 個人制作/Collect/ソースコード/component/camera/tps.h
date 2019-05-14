#pragma once

#include "..\..\framework\component.h"

#include <d3d11.h>
#include <DirectXMath.h>

class Entity;
class Graphics;
class Data;

class TPS : public Component
{
private:
	ID3D11Buffer* constant_buffer_ = nullptr;
	DirectX::XMFLOAT3 eye_ = { 0, 0, -2 };
	DirectX::XMFLOAT3 at_ = { 0, 0, 0 };
	DirectX::XMFLOAT3 up_ = { 0, 1, 0 };
	DirectX::XMFLOAT2 rot_ = { DirectX::XMConvertToRadians(-30), DirectX::XMConvertToRadians(0) };
	DirectX::XMFLOAT3 pos_ = { 0, 0, 0 };

	float radian_x_ = 0;
	float radian_y_ = 0;
	float move_ = 0;

	bool free_ = false;

public:
	TPS(Entity* parent, Data* data, float radian_x, float radian_y, float move);
	~TPS();

public:
	void SetPos(DirectX::XMFLOAT3 pos);

public:
	DirectX::XMMATRIX const GetView();
	float const GetRotX();
	float const GetRotY();

public:
	void Update(Graphics* graphics, Data* data);
	void Draw();
};
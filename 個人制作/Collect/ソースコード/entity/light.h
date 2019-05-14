#pragma once

#include "..\framework\entity.h"

#include <DirectXMath.h>


class Light : public Entity
{
private:
	DirectX::XMFLOAT4 light_ = { 0.1f, 8, -6, 0 };
	DirectX::XMFLOAT4 ambient_ = { 0, 0, 0, 1 };
	DirectX::XMFLOAT4 diffuse_ = { 0, 0, 0, 1 };
	DirectX::XMFLOAT4 specular_ = { 0, 0, 0, 1 };

public:
	Light(Scene* parent);
	~Light();

private:
	void Draw(RenderingPass pass) override;

public:
	void SetLightPos(DirectX::XMFLOAT3 pos);
};
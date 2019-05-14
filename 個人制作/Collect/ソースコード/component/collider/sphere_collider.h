#pragma once

#include "..\..\framework\component.h"

#include <DirectXMath.h>

class SphereCollider : public Component
{
private:
	DirectX::XMFLOAT3 position_ = { 0, 0, 0 };
	float radius_ = 0;

public:
	SphereCollider(Entity* parent) : Component(parent)
	{

	}
	~SphereCollider()
	{

	}

public:
	DirectX::XMFLOAT3& GetPosition()
	{
		return this->position_;
	}
	float& GetRadius()
	{
		return this->radius_;
	}
};
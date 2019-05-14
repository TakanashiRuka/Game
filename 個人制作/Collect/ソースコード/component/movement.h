#pragma once

#include "..\framework\component.h"

#include <DirectXMath.h>

class Movement : public Component
{
private:
	DirectX::XMFLOAT3 pos_ = { 25, 0, 25 };
	DirectX::XMFLOAT3 old_pos_;
	float rot_y_ = 0;

	float move_ = 0;
	float speed_ = 1;
	float degree_ = 0;


public:
	Movement(Entity* parent, float move, float degree);
	~Movement();

public:
	void Update();

public:
	void SetPosY(float pos_y);
	void SetSpeed(float speed);

public:
	DirectX::XMFLOAT3& GetPos();
	float GetSpeed();
	float const GetRotY();
	bool Move();
};
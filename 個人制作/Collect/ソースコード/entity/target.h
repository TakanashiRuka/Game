#pragma once

#include "..\framework\entity.h"

#include <DirectXMath.h>

class Target : public Entity
{
private:
	unsigned int texture_animation_ = 1;
	float uv_x_ = 0;
	float uv_w_ = 0;
	DirectX::XMFLOAT3 old_pos_ = { 0, 0, 0 };

	bool start_ = false;

public:
	Target(Scene* parent);
	~Target();

private:
	void Update() override;
	void Draw(RenderingPass pass) override;
};
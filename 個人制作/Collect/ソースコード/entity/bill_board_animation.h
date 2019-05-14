#pragma once

#include "..\framework\entity.h"
#include "..\framework\texture.h"

#include <DirectXMath.h>

enum class BillBoardAnimationType : unsigned int
{
	Blast,
	Impact,
};

class BillBoardAnimation : public Entity
{
private:
	unsigned int texture_animation_ = 1;
	unsigned int texture_animation_max_ = 1;
	float width_ = 0;
	float height_ = 0;
	float uv_x_ = 0;
	float uv_y_ = 0;
	float uv_w_ = 0;
	float uv_h_ = 0;
	BillBoardAnimationType type_;
	Texture texture_;
	DirectX::XMFLOAT3 pos_ = { 0, 0, 0 };
	DirectX::XMFLOAT4 color_ = { 1, 1, 1, 1 };

	bool wait_ = false;
	bool draw_ = true;

public:
	BillBoardAnimation(Scene* parent, BillBoardAnimationType type, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT4 color);
	~BillBoardAnimation();

private:
	void Update() override;
	void Draw(RenderingPass pass) override;
};
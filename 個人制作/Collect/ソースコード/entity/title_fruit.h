#pragma once

#include "..\framework\entity.h"

#include "..\framework\texture.h"

#include <DirectXMath.h>

class TitleFruit : public Entity
{
private:
	std::string name_;
	Texture texture_;
	DirectX::XMFLOAT4 out_line_color_ = { 1, 1, 1, 1 };
	unsigned int type_ = 0;
	float scaling_ = 0;
	float rotation_y_ = 0;
	float field_y_ = 0;

	bool fall_ = false;

public:
	TitleFruit(Scene* parent);
	~TitleFruit();

private:
	void Update() override;
	void Draw(RenderingPass pass) override;

public:
	DirectX::XMFLOAT4 GetOutLineColor();
};
#pragma once

#include "..\framework\entity.h"
#include "..\framework\texture.h"

#include <DirectXMath.h>

#include <string>

enum FruitType
{
	Apple,
	Banana,
	Kiwi,
	Pear,
	Strawberry,
};

class Fruit : public Entity
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
	bool draw_ = true;

public:
	Fruit(Scene* parent);
	~Fruit();

private:
	void Update() override;
	void Draw(RenderingPass pass) override;

public:
	Texture GetUseTexture();
	DirectX::XMFLOAT4 GetOutLineColor();
};
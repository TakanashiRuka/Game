#pragma once

#include "..\framework\entity.h"
#include "..\framework\texture.h"

#include <string>

class Theme : public Entity
{
private:
	std::string name_;
	Texture theme_;
	float scaling_ = 0;
	float rot_y_ = 0;

public:
	Theme(Scene* parent);
	~Theme();

private:
	void Update() override;
	void Draw(RenderingPass pass) override;

public:
	Texture GetThemeTexture();

public:
	void ChangeThemeTexture();
};
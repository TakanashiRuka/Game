#pragma once

#include "..\framework\entity.h"

class KeyButton : public Entity
{
private:
	float alpha_ = 1;
	float add_ = 0.02f;

public:
	KeyButton(Scene* parent);
	~KeyButton();

private:
	void Draw(RenderingPass pass) override;
};
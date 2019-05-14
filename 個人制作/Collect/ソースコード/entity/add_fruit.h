#pragma once

#include "..\framework\entity.h"

class AddFruit : public Entity
{
private:
	float alpha_ = 1;

public:
	AddFruit(Scene* parent);
	~AddFruit();

private:
	void Draw(RenderingPass pass) override;
};
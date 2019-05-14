#pragma once

#include "..\framework\entity.h"

class SpeedUp : public Entity
{
private:
	float alpha_ = 1;

public:
	SpeedUp(Scene* parent);
	~SpeedUp();

private:
	void Draw(RenderingPass pass) override;
};
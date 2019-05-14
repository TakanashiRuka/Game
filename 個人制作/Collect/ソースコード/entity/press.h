#pragma once

#include "..\framework\entity.h"

class Press : public Entity
{
private:
	float alpha_ = 1;
	float add_ = 0.02f;

public:
	Press(Scene* parent);
	~Press();

private:
	void Draw(RenderingPass pass) override;
};
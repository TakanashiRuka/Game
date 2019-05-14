#pragma once

#include "..\framework\entity.h"

class ChangeTitle : public Entity
{
private:
	float alpha_ = 0;

public:
	ChangeTitle(Scene* parent);
	~ChangeTitle();

public:
	void Draw(RenderingPass pass) override;
};
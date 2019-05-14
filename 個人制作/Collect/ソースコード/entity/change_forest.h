#pragma once

#include "..\framework\entity.h"

class ChangeForest : public Entity
{
private:
	float alpha_ = 0;

public:
	ChangeForest(Scene* parent);
	~ChangeForest();

public:
	void Draw(RenderingPass pass) override;
};
#pragma once

#include "..\framework\entity.h"

class ScoreUp : public Entity
{
private:
	float alpha_ = 1;

public:
	ScoreUp(Scene* parent);
	~ScoreUp();

private:
	void Draw(RenderingPass pass) override;
};
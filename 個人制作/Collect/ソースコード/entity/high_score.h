#pragma once

#include "..\framework\entity.h"

class HighScore : public Entity
{
private:
	unsigned int high_score_ = 0;

public:
	HighScore(Scene* parent);
	~HighScore();

private:
	void Draw(RenderingPass pass) override;
};
#pragma once

#include "..\framework\entity.h"

class Score : public Entity
{
private:
	unsigned int score_ = 0;

public:
	Score(Scene* parent);
	~Score();

private:
	void Update() override;
	void Draw(RenderingPass pass) override;

public:
	void AddScore(unsigned int score);
};
#pragma once

#include "..\framework\entity.h"

class ResultHighScore : public Entity
{
private:
	unsigned int high_score_ = 0;

public:
	ResultHighScore(Scene* parent);
	~ResultHighScore();

private:
	void Update() override;
	void Draw(RenderingPass pass) override;

public:
	unsigned int& GetHighScore();
};
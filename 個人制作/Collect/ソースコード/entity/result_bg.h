#pragma once

#include "..\framework\entity.h"

class ResultBG : public Entity
{
private:
	bool high_score_update_ = false;
	bool late_ = false;
	bool skip_ = false;

public:
	ResultBG(Scene* parent);
	~ResultBG();

private:
	void Update() override;
	void Draw(RenderingPass pass) override;
};
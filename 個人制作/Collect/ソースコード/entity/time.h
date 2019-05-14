#pragma once

#include "..\framework\entity.h"

class Time : public Entity
{
private:
	double time_ = 0;
	bool start_ = false;

public:
	Time(Scene* parent, double time);
	~Time();

private:
	void Update() override;
	void Draw(RenderingPass pass) override;
};
#pragma once

#include "..\framework\entity.h"

class TimeUp : public Entity
{
private:
	float ui_y_ = 0;
	double running_time_ = 0;

	bool time_up_ = false;
	bool add_entity_ = false;

public:
	TimeUp(Scene* parent);
	~TimeUp();

private:
	void Update() override;
	void Draw(RenderingPass pass) override;
};
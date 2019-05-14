#pragma once

#include "..\framework\entity.h"

class Player : public Entity
{
private:
	double animation_running_time_ = 0;

public:
	Player(Scene* parent);
	~Player();

private:
	void Update() override;
	void Draw(RenderingPass pass) override;
};
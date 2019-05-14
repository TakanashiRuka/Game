#pragma once

#include "..\framework\entity.h"

class Bomb : public Entity
{
private:
	double apply_time_ = 0;

	bool use_ = false;

public:
	Bomb(Scene* parent);
	~Bomb();

private:
	void Update() override;
	void Draw(RenderingPass pass) override;
};
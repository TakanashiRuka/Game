#pragma once

#include "..\framework\entity.h"

class Sprint : public Entity
{
private:
	double apply_time_ = 0;

	bool use_ = false;

public:
	Sprint(Scene* parent);
	~Sprint();

private:
	void Update() override;
	void Draw(RenderingPass pass) override;
};
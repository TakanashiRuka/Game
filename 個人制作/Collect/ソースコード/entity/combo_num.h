#pragma once

#include "..\framework\entity.h"

class ComboNum : public Entity
{
private:
	unsigned int old_combo_ = 0;
	float alpha_ = 1;

public:
	ComboNum(Scene* parent);
	~ComboNum();

private:
	void Draw(RenderingPass pass) override;
};
#pragma once

#include "..\framework\entity.h"

class Combo : public Entity
{
private:
	unsigned int combo_cnt_ = 0;
	float alpha_ = 1;

public:
	Combo(Scene* parent);
	~Combo();

private:
	void Draw(RenderingPass pass) override;

public:
	void AddCombo();
	unsigned int GetCombo();
	void ResetCombo();
};
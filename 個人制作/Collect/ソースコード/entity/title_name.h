#pragma once

#include "..\framework\entity.h"

class TitleName : public Entity
{
private:
	double running_time_ = 0;

public:
	TitleName(Scene* parent);
	~TitleName();

private:
	void Update() override;
	void Draw(RenderingPass pass) override;
};
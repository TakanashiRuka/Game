#pragma once

#include "..\framework\entity.h"

class TimeBG : public Entity
{
public:
	TimeBG(Scene* parent);
	~TimeBG();

private:
	void Draw(RenderingPass pass) override;
};
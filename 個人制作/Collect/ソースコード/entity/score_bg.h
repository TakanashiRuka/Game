#pragma once

#include "..\framework\entity.h"

class ScoreBG : public Entity
{
public:
	ScoreBG(Scene* parent);
	~ScoreBG();

private:
	void Draw(RenderingPass pass) override;
};
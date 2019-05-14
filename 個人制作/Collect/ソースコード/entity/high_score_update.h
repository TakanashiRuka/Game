#pragma once

#include "..\framework\entity.h"

class HighScoreUpdate : public Entity
{
public:
	HighScoreUpdate(Scene* parent);
	~HighScoreUpdate();

private:
	void Draw(RenderingPass pass) override;
};
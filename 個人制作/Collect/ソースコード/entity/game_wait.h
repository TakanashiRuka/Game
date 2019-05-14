#pragma once

#include "..\framework\entity.h"

class GameWait : public Entity
{
public:
	GameWait(Scene* parent);
	~GameWait();

private:
	void Update() override;
	void Draw(RenderingPass pass) override;
};
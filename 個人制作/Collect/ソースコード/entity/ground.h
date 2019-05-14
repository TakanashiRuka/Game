#pragma once

#include "..\framework\entity.h"

class Ground : public Entity
{
public:
	Ground(Scene* parent);
	~Ground();

private:
	void Update() override;
	void Draw(RenderingPass pass) override;
};
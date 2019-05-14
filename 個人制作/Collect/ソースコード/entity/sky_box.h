#pragma once

#include "..\framework\entity.h"

class SkyBox : public Entity
{
public:
	SkyBox(Scene* parent);
	~SkyBox();

private:
	void Always() override;
	void Draw(RenderingPass pass) override;
};
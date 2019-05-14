#pragma once

#include "..\framework\entity.h"

class Operation : public Entity
{
public:
	Operation(Scene* parent);
	~Operation();

private:
	void Update() override;
	void Draw(RenderingPass pass) override;
};
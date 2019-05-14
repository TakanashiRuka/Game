#pragma once

#include "..\framework\entity.h"

#include <DirectXMath.h>

class TitleCamera : public Entity
{
private:
	float rot_x_ = 0;
	float rot_y_ = 0;

public:
	TitleCamera(Scene* parent);
	~TitleCamera();

private:
	void Update() override;

public:
	float GetRotX();
	float GetRotY();
	DirectX::XMMATRIX GetView();
};
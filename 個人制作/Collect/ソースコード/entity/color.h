#pragma once

#include "..\framework\entity.h"

#include <DirectXMath.h>

class Color : public Entity
{
public:
	Color(Scene* parent);
	~Color();

public:
	void SetColor(DirectX::XMFLOAT4 color);
};
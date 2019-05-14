#pragma once

#include "..\framework\entity.h"

constexpr unsigned int field_width = 50;
constexpr unsigned int field_height = 50;

class MeshField : public Entity
{
public:
	MeshField(Scene* parent);
	~MeshField();

private:
	void Draw(RenderingPass pass) override;
};
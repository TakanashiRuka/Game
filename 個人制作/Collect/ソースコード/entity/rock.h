#pragma once

#include "..\framework\entity.h"

#include <DirectXMath.h>

#include <vector>

class Rock : public Entity
{
private:
	struct VertexBufferInstancedData
	{
		float rot_y;
		DirectX::XMFLOAT3 position;
	};
	std::vector<VertexBufferInstancedData> vbi_data_;

public:
	Rock(Scene* parent);
	~Rock();

private:
	void Update() override;
	void Draw(RenderingPass pass) override;
};
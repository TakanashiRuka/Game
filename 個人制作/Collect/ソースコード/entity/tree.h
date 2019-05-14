#pragma once

#include "..\framework\entity.h"

#include <DirectXMath.h>

#include <vector>

class Tree : public Entity
{
private:
	struct VertexBufferInstancedData
	{
		float rot_y;
		DirectX::XMFLOAT3 position;
	};
	std::vector<VertexBufferInstancedData> vbi_data_;

public:
	Tree(Scene* parent);
	~Tree();

private:
	void Update() override;
	void Draw(RenderingPass pass) override;
};
#pragma once

#include "..\..\framework\component.h"

#include <d3d11.h>

#include <vector>

class Entity;
class Graphics;

class DrawIndexed : public Component
{
private:
	unsigned int mesh_size_ = 0;

public:
	DrawIndexed(Entity* parent, unsigned int mesh_size);

public:
	void Draw(Graphics* graphics, ID3D11Buffer* constant_buffer, std::vector<ID3D11Buffer*> vertex_buffers, std::vector<ID3D11Buffer*> index_buffers, std::vector<unsigned int> index_size);
	void DrawAnimation(Graphics* graphics, std::vector<ID3D11Buffer*> constant_buffers, std::vector<ID3D11Buffer*> vertex_buffers, std::vector<ID3D11Buffer*> index_buffers, std::vector<unsigned int> index_size);
};
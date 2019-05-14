#pragma once

#include "..\..\framework\component.h"
#include "..\..\model\model_data.h"

class MeshCollider : public Component
{
private:
	std::vector<Mesh> meshes_;

public:
	MeshCollider(Entity* parent, Model& model) : Component(parent)
	{
		this->meshes_ = model.meshes;
	}
	~MeshCollider()
	{

	}

public:
	std::vector<Mesh>& GetMeshes()
	{
		return this->meshes_;
	}
};
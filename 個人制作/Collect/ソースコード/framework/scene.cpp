#include "..\framework\scene.h"
#include "..\framework\entity.h"
#include "..\framework\data.h"

Scene::Scene()
{
	this->data_ = new Data;
}

Scene::~Scene()
{
	for (auto& entity : this->entities_)
		delete entity;

	delete this->data_;
}

std::vector<Entity*> const Scene::GetEntities()
{
	return this->entities_;
}

void Scene::Update()
{
	for (size_t n = 0; n < this->entities_.size();)
	{
		auto& entity = this->entities_[n];

		(entity->IsRemove())
			? this->RemoveEntity(entity, n)
			: (entity->Update(), ++n);
	}
}

void Scene::Pause()
{
	for (size_t n = 0; n < this->entities_.size();)
	{
		auto& entity = this->entities_[n];

		(entity->IsRemove())
			? this->RemoveEntity(entity, n)
			: (entity->Pause(), ++n);
	}
}

void Scene::Always()
{
	for (size_t n = 0; n < this->entities_.size();)
	{
		auto& entity = this->entities_[n];

		(entity->IsRemove())
			? this->RemoveEntity(entity, n)
			: (entity->Always(), ++n);
	}
}

void Scene::Draw(RenderingPass pass)
{
	for (size_t n = 0; n < this->entities_.size();)
	{
		auto& entity = this->entities_[n];

		(entity->IsRemove())
			? this->RemoveEntity(entity, n)
			: (entity->Draw(pass), ++n);
	}
}

Data* const Scene::GetData()
{
	return this->data_;
}

void Scene::RemoveEntity(Entity*& entity, size_t n)
{
	delete entity;
	this->entities_.erase(this->entities_.begin() + n);
}
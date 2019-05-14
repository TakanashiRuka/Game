#pragma once

#include "..\framework\rendering_pass.h"

#include <unordered_map>
#include <vector>

class Data;

class Scene
{
	friend class Game;
	friend class Entity;

private:
	Data* data_ = nullptr;
	std::vector<Entity*> entities_;

public:
	Scene();
	virtual ~Scene();

public:
	template<class T, class ... Args>
	void AddEntity(Args ... args);

public:
	std::vector<Entity*> const GetEntities();

private:
	void Update();
	void Pause();
	void Always();
	void Draw(RenderingPass pass);

private:
	Data* const GetData();
	void RemoveEntity(Entity*& entity, size_t n);
};

template<class T, class ...Args>
inline void Scene::AddEntity(Args ...args)
{
	this->entities_.emplace_back(new T(this, args ...));
}
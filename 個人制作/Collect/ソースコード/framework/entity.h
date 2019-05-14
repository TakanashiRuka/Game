#pragma once

#include "..\framework\component.h"

#include "..\framework\rendering_pass.h"

#include <d3d11.h>

#include <string>
#include <unordered_map>
#include <typeindex>

class Graphics;
class Data;

class Entity
{
	friend class Scene;

private:
	std::unordered_map<std::type_index, Component*> components_;

	bool remove_ = false;

protected:
	Scene* parent_ = nullptr;
	Graphics* graphics_ = nullptr;
	Data* data_ = nullptr;
	std::string tag_;
	ID3D11Buffer* constant_buffer_ = nullptr;
	std::vector<ID3D11Buffer*> constant_buffers_;

public:
	Entity(Scene* parent);
	virtual ~Entity();

public:
	template<class T, class ... Args>
	void AddComponent(Args ... args);
	template<class T>
	void RemoveComponent();
	template<class T>
	bool HasComponent();
	template<class T>
	T* const GetComponent();

public:
	bool FindWithTag(std::string tag);

public:
	bool IsRemove();
	void Remove();

private:
	virtual void Update() {}
	virtual void Pause() {}
	virtual void Always() {}
	virtual void Draw(RenderingPass) {}
};

template<class T, class ... Args>
inline void Entity::AddComponent(Args ... args)
{
	if (!this->HasComponent<T>())
	{
		this->components_[typeid(T)] = new T(this, args ...);
	}
}

template<class T>
inline void Entity::RemoveComponent()
{
	if (this->HasComponent<T>())
	{
		delete this->components_[typeid(T)];
		this->components_.erase(typeid(T));
	}
}

template<class T>
inline bool Entity::HasComponent()
{
	if (this->components_[typeid(T)])
	{
		return true;
	}
	else
	{
		return false;
	}
}

template<class T>
inline T* const Entity::GetComponent()
{
	if (this->HasComponent<T>())
	{
		return static_cast<T*>(this->components_[typeid(T)]);
	}
	else
	{
		return nullptr;
	}
}
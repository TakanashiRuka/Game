#pragma once

class Component
{
	friend class Entity;

private:
	bool remove_ = false;

protected:
	Entity* parent_ = nullptr;

public:
	Component(Entity* parent);
	virtual ~Component();
};
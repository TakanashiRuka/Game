#pragma once

#include "..\framework\entity.h"

class FruitIndex : public Entity
{
private:
	int index_ = 0;

public:
	FruitIndex(Scene* parent) : Entity(parent)
	{
		this->tag_ = "fruit index";
	}
	~FruitIndex()
	{

	}

public:
	int GetIndex()
	{
		return this->index_;
	}
	void AddIndex()
	{
		++this->index_;
	}
};
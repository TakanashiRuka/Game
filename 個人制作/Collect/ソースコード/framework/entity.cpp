#include "..\framework\entity.h"

#include "..\framework\game.h"
#include "..\framework\utility.h"
#include "..\graphics\directx11.h"

Entity::Entity(Scene* parent) : parent_(parent)
{
	this->graphics_ = Game::GetInstance()->GetGraphics();
	this->data_ = this->parent_->GetData();
}

Entity::~Entity()
{
	for (auto& component : this->components_)
		delete component.second;

	SafeRelease(this->constant_buffer_);

	for (auto& constant_buffer : this->constant_buffers_)
		SafeRelease(constant_buffer);
}

bool Entity::FindWithTag(std::string tag)
{
	return (this->tag_ == tag);
}

bool Entity::IsRemove()
{
	return this->remove_;
}

void Entity::Remove()
{
	this->remove_ = true;
}
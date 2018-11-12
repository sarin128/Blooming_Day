#include"pch.h"
#include"Component.h"


Component::Component(Entity *owner) : 
	owner_(owner), isEnabled_(true) 
{}
Component::~Component() 
{}

void Component::SetEnabled(bool flag)
{
	if (isEnabled_ != flag)
	{
		isEnabled_ = flag;
		isEnabled_ ? OnEnabled() : OnDisabled();
	}
}

bool Component::IsEnabled(void) const
{
	return isEnabled_;
}

Entity* Component::GetOwner(void) const
{
	return owner_;
}
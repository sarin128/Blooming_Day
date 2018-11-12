#pragma once
#include"Entity.h"
class Component
{
private:
	Entity* owner_;
	bool isEnabled_;
public:
	Component(Entity* owner);
	virtual ~Component();

	void SetEnabled(bool flag);
	bool IsEnabled() const;

	Entity* GetOwner() const;

	virtual void Reset() {} 
	virtual std::type_index GetID(void) = 0 { return typeid(Component); }
	static std::type_index GetFamilyID(void) { return typeid(Component); }

	virtual void OnEnabled(void) {}
	virtual void OnDisabled(void) {}
	virtual void OnUpdate(void) {}
	virtual void OnRender(void) {}
};


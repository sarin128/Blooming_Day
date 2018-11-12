#pragma once
#include "Component.h"
class Collider :
	public Component
{
private:
	friend class Scene;
	std::function<void(Entity*)> onCollision_;
public:
	Collider(Entity* owner);
	virtual ~Collider() override;
	
public:
	virtual void Reset(void) override {}
	virtual std::type_index GetID(void) override = 0 { return typeid(Collider); }
	static std::type_index GetFamilyID(void) { return typeid(Collider); }

	virtual void OnUpdate(void) override {}
	void OnEnabled(void) override;
	void OnDisabled(void) override;
	Collider* SetOnCollisionListener(std::function<void(Entity*)> callback);
protected:
	virtual bool IsCollision(Collider* other) = 0 {}
};


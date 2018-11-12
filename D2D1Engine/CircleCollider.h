#pragma once
#include "Collider.h"
#include"Math.h"
class CircleCollider :
	public Collider
{
public:
	Circle circle;

public:
	CircleCollider(Entity* owner,const Circle& circle = Circle());
	virtual ~CircleCollider() override;

	void Reset(void) {};
	std::type_index GetID(void) override { return typeid(CircleCollider); }
	static std::type_index GetFamilyID(void) { return typeid(Collider); }

	void OnUpdate(void) {};



	Circle GetCircleWithWorldPos() const;
	Circle GetCircleWithPos() const;
protected:

	bool IsCollision(Collider* other) override;
};


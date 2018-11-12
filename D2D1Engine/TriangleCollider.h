#pragma once
#include "Collider.h"
#include "Math.h"
class TriangleCollider :
	public Collider
{
public:
	Triangle triangle;

public:
	TriangleCollider(Entity* owner, const Triangle& triangle = Triangle());
	virtual ~TriangleCollider() override;

	void Reset(void) {};
	std::type_index GetID(void) override { return typeid(TriangleCollider); }
	static std::type_index GetFamilyID(void) { return typeid(Collider); }

	void OnUpdate(void) {};



	Circle GetCircleWithWorldPos() const;
	Circle GetCircleWithPos() const;
protected:

	bool IsCollision(Collider* other) override;
};


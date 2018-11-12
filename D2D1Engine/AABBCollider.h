#pragma once
#include "Collider.h"
#include"Math.h"
class AABBCollider :
	public Collider
{
public:
	Rect rect;

public:
	AABBCollider(Entity* owner, const Rect& rect = Rect());
	virtual ~AABBCollider() override {}

	void Reset(void);
	std::type_index GetID(void) override { return typeid(AABBCollider); }
	static std::type_index GetFamilyID(void) { return typeid(Collider); }

	void OnUpdate(void);

	AABBCollider* SetRect(const Rect& rect) { this->rect = rect; return this; }
	Rect GetRect() const { return rect; }

	Rect GetRectWithWorldPos() const;
	Rect GetRectWithPos() const;
protected:

	bool IsCollision(Collider* other) override;
};


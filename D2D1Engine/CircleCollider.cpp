#include "pch.h"
#include "CircleCollider.h"
#include"AABBCollider.h"

CircleCollider::CircleCollider(Entity * owner, const Circle & circle)
	:Collider(owner), circle(circle)
{
}

CircleCollider::~CircleCollider()
{
}

Circle CircleCollider::GetCircleWithWorldPos() const
{
	return circle.Offset(GetOwner()->GetWorldPos());
}

Circle CircleCollider::GetCircleWithPos() const
{
	return circle.Offset(GetOwner()->pos);
}

bool CircleCollider::IsCollision(Collider* other)
{
	if (other->GetID() == typeid(CircleCollider))
	{
		const Circle& c1 = GetCircleWithWorldPos();
		const Circle& c2 = ((CircleCollider*)other)->GetCircleWithWorldPos();
		return c1.center.Distance(c2.center) < c1.radius + c2.radius;
	}
	if (other->GetID() == typeid(AABBCollider))
	{
		
		const Circle& c1 = GetCircleWithWorldPos();
		const Rect& r2 = ((AABBCollider*)other)->GetRectWithWorldPos();
		Vec2F nearest(
			Clamp(c1.center.x, r2.left, r2.right),
			Clamp(c1.center.y, r2.top, r2.bottom));

		

		return nearest.DistanceSquare(c1.center) <= c1.radius * c1.radius;
	}
	return false;
}

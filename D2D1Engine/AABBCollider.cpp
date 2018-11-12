#include "pch.h"
#include "AABBCollider.h"
#include"Entity.h"
#include"App.h"
#include"Scene.h"
#include"SpriteRenderer.h"
#include"AnimationRenderer.h"
#include"CircleCollider.h"
AABBCollider::AABBCollider(Entity* owner, const Rect& rect)
	:Collider(owner),
	rect(rect)
{
	if (this->rect == Rect())
	{
		if(owner->GetComponent<SpriteRenderer>())
			this->rect = owner->GetComponent<SpriteRenderer>()->visibleRect;
		else if(owner->GetComponent<AnimationRenderer>())
			this->rect = owner->GetComponent<AnimationRenderer>()->visibleRect;
	}
}

bool AABBCollider::IsCollision(Collider * other)
{

	if (other->GetID() == typeid(AABBCollider))
	{
		return GetRectWithWorldPos().IsIntersect(((AABBCollider*)other)->GetRectWithWorldPos());
	}
	else if (other->GetID() == typeid(CircleCollider))
	{
		const Circle& c1 = ((CircleCollider*)other)->GetCircleWithWorldPos();
		const Rect& r2 = GetRectWithWorldPos();
		Vec2F nearest(
			Clamp(c1.center.x, r2.left, r2.right),
			Clamp(c1.center.y, r2.top, r2.bottom));
		return nearest.DistanceSquare(c1.center) <= c1.radius * c1.radius;
	}
	return false;
}

void AABBCollider::Reset(void)
{
	rect = Rect();
}

void AABBCollider::OnUpdate(void)
{

}

Rect AABBCollider::GetRectWithWorldPos() const
{
	return rect.Offset(GetOwner()->GetWorldPos());
}

Rect AABBCollider::GetRectWithPos() const
{
	return rect.Offset(GetOwner()->pos);
}



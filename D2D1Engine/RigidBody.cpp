#include "pch.h"
#include "RigidBody.h"

RigidBody::RigidBody(Entity * owner, bool isFixed, bool isGravityEnabled)
	:Component(owner), isFixed(isFixed), isGravityEnabled(isGravityEnabled),
	gravity(0.f),
	isTouchingBottom(false),
	isTouchingTop(false),
	isTouchingRight(false),
	isTouchingLeft(false)
{
}

RigidBody::~RigidBody()
{
}

void RigidBody::OnUpdate(void)
{
	isTouchingBottom = false;
	if (!isFixed && isGravityEnabled)
	{
		gravity += AG_PER_DT;
		GetOwner()->pos.y += gravity;
	}
}


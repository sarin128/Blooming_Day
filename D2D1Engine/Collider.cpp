#include "pch.h"
#include "Collider.h"
#include"App.h"
#include"Scene.h"
Collider::Collider(Entity * owner)
	:Component(owner),
	onCollision_([](Entity*) { })
{
	App::GetInstance()->GetSceneManager()->GetScene()->RegisterCollidable(owner);
}

Collider::~Collider()
{
	App::GetInstance()->GetSceneManager()->GetScene()->UnregisterCollidable(GetOwner());
}

void Collider::OnEnabled(void)
{
	App::GetInstance()->GetSceneManager()->GetScene()->RegisterCollidable(GetOwner());
}

void Collider::OnDisabled(void)
{
	App::GetInstance()->GetSceneManager()->GetScene()->UnregisterCollidable(GetOwner());
}

Collider* Collider::SetOnCollisionListener(std::function<void(Entity*)> callback)
{
	onCollision_ = callback;
	return this;
}

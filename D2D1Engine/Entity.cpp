#include "pch.h"
#include "Entity.h"
#include"Component.h"
#include"App.h"
#include"Scene.h"

Entity::Entity(bool isRegistered)
	:isEnabled_(true),
	isRegistered_(isRegistered),
	lifeState_(Alive),
	tag(None),
	name("NoName"),
	parent_(nullptr),
	matrix(),
	pos(0.f, 0.f),
	scale(1.f, 1.f),
	center(0.f, 0.f),
	rotation(0.f),
	scaleCenter(0.f, 0.f),
	rotationCenter(0.f, 0.f),
	skewCenter(0.f, 0.f),
	skew(0.f, 0.f),
	//color(1.f,1.f,1.f,1.f),
	isAutoRenderEnabled(true),
	isAutoUpdateEnabled(true),
	isFollowingParentEnabled(true)
{
	if(isRegistered)
		App::GetInstance()->GetSceneManager()->GetScene()->RegisterEntity(this);
}


Entity::~Entity()
{
	if(isRegistered_)
		App::GetInstance()->GetSceneManager()->GetScene()->UnregisterEntity(this);
	for (auto& i : components_)
		delete i.second;
	for (auto& i : children_)
		delete i;
}

void Entity::SetEnabled(bool flag)
{
	if (isEnabled_ == flag) return;
	
	isEnabled_ = flag;
	isEnabled_ ? OnEnabled() : OnDisabled();
	
}

bool Entity::IsEnabled() const
{
	return isEnabled_;
}


Vec2F Entity::GetWorldPos() const
{
	return Vec2F(matrix._31, matrix._32);
}

Vec2F Entity::GetWorldPosByCenter() const
{
	return Vec2F(matrix._31, matrix._32) - center;
}

void Entity::AddChild(Entity * child)
{
	child->parent_ = this;
	children_.push_back(child);
}

void Entity::RemoveChild(Entity * child)
{
	child->Destroy();
}

void Entity::DetachChild(Entity * child)
{
	child->Detach();
}

list<Entity*> Entity::GetChild() {
	return children_;
}

Entity* Entity::GetParent() const
{
	return parent_;
}

void Entity::Update()
{


	for (auto i : components_)
	{
		if ((i.second)->IsEnabled())
		{
			(i.second)->OnUpdate();
		}
	}

	if (isAutoUpdateEnabled)
	{
		for (auto i : children_)
		{
			if (i->IsEnabled())
			{
				i->Update();
			}
				
		}
	}

	
	OnUpdate();
	
	matrix = D2D1::Matrix3x2F::Translation(pos.x - center.x, pos.y - center.y);

	if (parent_&&isFollowingParentEnabled)
	{
		matrix = matrix * parent_->matrix;
	}

	children_.erase(std::remove_if(children_.begin(), children_.end(), [](Entity* element) {
		const LifeState tmp = element->GetLifeState();
		if (tmp == Destroyed)
		{	
			delete element;
			return true;
		}
		else if (tmp == Detached)
		{
			element->parent_ = nullptr;
			element->lifeState_ = Alive;
			return true;
		}
		return false;
	}), children_.end());

}

void Entity::Render()
{
	matrix =
		D2D1::Matrix3x2F::Skew(skew.x, skew.y, skewCenter) *
		D2D1::Matrix3x2F::Scale(scale.x, scale.y, scaleCenter) *
		D2D1::Matrix3x2F::Rotation(rotation, rotationCenter) *
		D2D1::Matrix3x2F::Translation(pos.x - center.x, pos.y - center.y);

	if (parent_&&isFollowingParentEnabled)
	{
		matrix = matrix * parent_->matrix;
	}

	App::GetInstance()->GetGraphics()->GetDeviceContext()->SetTransform(matrix);

	for (auto i : components_)
	{
		if ((i.second)->IsEnabled())
		{
			(i.second)->OnRender();
		}
	}

	if (isAutoRenderEnabled)
	{
		for (auto i : children_)
		{
			if (i->IsEnabled())
				i->Render();
		}
	}

	OnRender();
}

void Entity::Destroy()
{
	lifeState_ = Destroyed;
}

void Entity::Detach()
{
	lifeState_ = Detached;
}

void Entity::GetL(std::function<void()> callback) {
	callback();
}

void Entity::SetHp(Tag tag) {
	switch(tag) {
	case None: this->hp = 0; break;
	case TagTemple:
	case TagTower:this->hp = 3000; break;
	case TagNexus:this->hp = 7500; break;
	case TagWarrior:this->hp = 350; break;
	case TagGuardian:this->hp = 1000; break;
	case TagArcher: this->hp = 200; break;
	case TagWBarrack:
	case TagABarrack:
	case TagGBarrack:this->hp = 2500; break;
	case TagBuildingGround:this->hp = 0; break;
	default:break;
	}
}

void Entity::PathFinding(Vec2F TargetPos) {
	Path = TargetPos;
}
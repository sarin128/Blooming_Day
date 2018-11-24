#pragma once
#include"Math.h"
class Scene;
class Component;
// 
enum LifeState {
	Alive,
	Destroyed,
	Detached
};
class Entity
{
private:
	friend class Scene;
	std::map<std::type_index, Component*> components_;
	Entity* parent_;
	std::list<Entity*> children_;
	bool isRegistered_;
	bool isEnabled_;
	LifeState lifeState_;
public:
	enum Price {
		Barrack = 1500,
		Tower = 1000,
		Temple = 750,
	};
	Tag tag;
	//deprecated
	std::string name;

	Matrix matrix;
	
	Vec2F scale;
	Vec2F scaleCenter;

	Vec2F pos;
	Vec2F center;

	Degree rotation;
	Vec2F rotationCenter;

	Vec2F skew;
	Vec2F skewCenter;

	float hp = 0;

	//Color color;

	bool isAutoRenderEnabled;
	bool isAutoUpdateEnabled;
	bool isFollowingParentEnabled;
public:
	Entity(bool isRegistered = true);
	virtual ~Entity();
	void SetEnabled(bool flag);
	bool IsEnabled() const;
	int BuildingUpgrade = 1;
	int UnitUpgrade = 1;
	template<typename T, typename... Args>
	T* AttachComponent(Args&&... args);
	template<typename T>
	void DetachComponent(void);
	template<typename T>
	T* GetComponent(void);

	//Vec2F GetPosByCenter() const;
	//void SetPosByCenter(Vec2F pos);
	Vec2F GetWorldPos() const ;
	Vec2F GetWorldPosByCenter() const;

	void AddChild(Entity* child);
	void RemoveChild(Entity* child);
	void DetachChild(Entity* child);

	list<Entity*> GetChild();

	Entity* GetParent() const;

	void SetHp(Tag tag);

	void Update();
	void Render();
	void Destroy();
	void Detach();

	bool IsDestroyed() const { return lifeState_ == Destroyed; }
	bool IsDetached() const { return lifeState_ == Detached; }
	LifeState GetLifeState() const { return lifeState_; }

	//Tag tag;
	////deprecated
	//std::string name;

	//Matrix matrix;

	//Vec2F scale;
	//Vec2F scaleCenter;

	//Vec2F pos;
	//Vec2F center;

	//Degree rotation;
	//Vec2F rotationCenter;

	//Vec2F skew;
	//Vec2F skewCenter;

	//bool isAutoRenderEnabled;
	//bool isAutoUpdateEnabled;
	//bool isFollowingParentEnabled;
	Entity* SetTag(Tag tag) { this->tag = tag; return this; }
	Entity* SetScale(const Vec2F& scale) { this->scale = scale; return this;}
	Entity* SetScaleCenter(const Vec2F& center) {this->scaleCenter = center; return this;}
	Entity* SetRotation(float rotation) { this->rotation = rotation; return this;}
	Entity* SetRotationCenter(const Vec2F& center) { this->rotationCenter = center;  return this;}
	Entity* SetPos(const Vec2F& pos) { this->pos = pos;  return this;}
	Entity* SetCenter(const Vec2F& center) { this->center = center; return this;}
	Entity* SetAutoRender(bool flag) { isAutoRenderEnabled = flag; return this;}
	Entity* SetAutoUpdate(bool flag) { isAutoUpdateEnabled = flag; return this;}
	Entity* SetFollowingParent(bool flag) { isFollowingParentEnabled = flag;  return this;}
	float GetHp() { return this->hp; }
	void PathFinding(Vec2F TargetPos);
	Vec2F Path;

	Tag GetTag() const { return tag; }
	Vec2F GetScale() const { return scale; }
	Vec2F GetScaleCenter()const { return scaleCenter; }
	float GetRotation() const { return rotation; }
	Vec2F GetRotationCenter() const { return rotationCenter; }
	Vec2F GetPos() const { return pos; }
	Vec2F GetCenter() const { return center; }
	bool IsAutoRenderEnabled() const { return isAutoRenderEnabled; }
	bool IsAutoUpdateEnabled() const { return isAutoUpdateEnabled; }
	bool IsFollowingParentEnabled() const { return isFollowingParentEnabled; }

	virtual void OnEnabled(void) {}
	virtual void OnDisabled(void) {}

	virtual void OnUpdate(void) {}
	virtual void OnRender(void) {}

	void GetL(std::function<void()> callback);
	//virtual void OnCollision(Entity* other) {}
};


template<typename T, typename... Args>
T* Entity::AttachComponent(Args&&... args)
{
	std::type_index i(T::GetFamilyID());
	if (components_.count(i) > 0)
		delete components_[i];

	T* component = new T(this, args...);
	components_[i] = component;

	return component;
}
template<typename T>
void Entity::DetachComponent(void)
{
	std::type_index i(T::GetFamilyID());
	Component* component = components_[i];
	delete component;
	components_.erase(i);
}
template<typename T>
T* Entity::GetComponent(void)
{
	std::type_index i(T::GetFamilyID());
	if (components_.count(i) <= 0)
		return nullptr;

	return static_cast<T*>(components_[i]);
}

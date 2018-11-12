#pragma once
#include"Entity.h"
#include"Camera.h"
class Scene:
	public Entity
{
private:
	friend class Entity;
	friend class SceneManager;
	friend class Collider;

	static bool isChanging_;
	static std::vector<Entity*> tempEntities_;
	static std::vector<Entity* > tempCollidables_;
	std::vector<Entity* > entities_;
	std::vector<Entity* > collidables_;

	

	void RegisterEntity(Entity* e);
	void UnregisterEntity(Entity* e);
	void RegisterCollidable(Entity* e);
	void UnregisterCollidable(Entity* e);

	void Dispose();
public:
	Camera camera;
public:
	Scene();
	virtual ~Scene();

	const std::vector<Entity*>& GetEntities(void) const;

	int GetEntitiesCount(void) const { return entities_.size(); }

	void Render();
	void Update();

	virtual void OnUpdate() override;
	
	//해당 태그를 가진 엔티티를 찾는다.
	//없을시 nullptr
	Entity* FindEntity(Tag tag);
	//해당 조건을 만족하는 엔티티를 찾는다.
	//없을시 nullptr
	Entity* FindEntityIf(std::function<bool(Entity*)> pred);
};



#pragma once
#include"pch.h"
#include "Scene.h"
#include "Button.h"
class Scene1 :
	public Scene
{
public:
	Scene1();
	~Scene1();
	void OnUpdate();
	Button* FaithView;
	string faith = "½Å¾Ó½É : ";
	float Faith;
	vector<Button*>UI;
	Entity* player; Camera* cam;
	Entity* minimap;
	void CameraMove(const Vec2F Pos);
	void SetMouseListener(Entity* entity);
	void SetMinimap(bool set);
	void SetAllyBase();
	void SetEnemyBase();
	void SetUI();
	void UpgradeBuilding(Tag tag, int Upgrade);
	void UpgradeUnit(Tag tag, int Upgrade);
	void ButtonSetting(vector<Button*> UI);
	vector<Entity*>Ally;
	vector<Entity*>Enemy;
	list<Entity*>AllyUnit;
	list<Entity*>EnemyUnit;
	D2D1_ELLIPSE* ellipse;
	Button* button;
	bool IsMinimapEnabled = false;
	void TowerButton(vector<Button*> UI);
	void GBarruckButton(vector<Button*> UI);
	void WBarruckButton(vector<Button*> UI);
	void ABarruckButton(vector<Button*> UI);
	void TempleButton(vector<Button*> UI);
	void DefaultButton(vector<Button*> UI);
	void ChangeBuilding(Entity* building, Tag tag);
};
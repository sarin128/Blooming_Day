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
	LPCWSTR CursorPath;
	HCURSOR SetCursorImage;
	Button* FaithView;
	Button* HpView;
	string faith = "½Å¾Ó½É : ";
	string targetHp = "¼±ÅÃ À¯´Ö Ã¼·Â : ";
	float Faith;
	Tag tag;
	vector<Button*>UI;
	Entity* Player; Camera* cam;
	Entity* minimap;
	void CameraMove(const Vec2F Pos);
	void SetMouseListener(Entity* entity);
	void SetMinimap(bool set);
	void SetAllyBase();
	void SetEnemyBase();
	void SetNeutralityBase();
	void SetUI();
	void UpgradeBuilding(Tag tag, int Upgrade);
	void PutDownBuilding(Entity* Building, int Upgrade);
	void ButtonSetting(vector<Button*> UI);
	vector<Entity*>Ally;
	vector<Entity*>Enemy;
	vector<Entity*>Neutrality;
	list<Entity*>AllyUnit;
	list<Entity*>EnemyUnit;
	bool IsMinimapEnabled = false;
	void TowerButton(vector<Button*> UI);
	void GBarruckButton(vector<Button*> UI);
	void WBarruckButton(vector<Button*> UI);
	void ABarruckButton(vector<Button*> UI);
	void TempleButton(vector<Button*> UI);
	void DefaultButton(vector<Button*> UI);
	void ChangeBuidingButtion(vector<Button*> UI);
	void ChangeBuilding(Entity* building, Tag tag);
	void CreateBuilding(Entity* building, Tag tag);
	void SetMainCursor();
	void SetTargetCursor();
};
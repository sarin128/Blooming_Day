#include "pch.h"
#include "Scene1.h"
#include "testEntity.h"
#include "RigidBody.h"
#include "SpriteRenderer.h"
#include "AABBCollider.h"
#include "Camera.h"
#include "MouseListener.h"
#include "Window.h"
Scene1::Scene1()
{

	Ally.resize(6);
	Enemy.resize(6);
	UI.resize(5);
	player = new testEntity();
	AddChild(player);

	SetAllyBase();
	SetEnemyBase();
	camera.UnFollow();
	camera.SetSmoothFollowEnabled(true);
	camera.zoom = 0.5f;

	Faith = 3000;
	FaithView = new Button("Text");
	camera.SetFaithScore(FaithView->pos);
	AddChild(FaithView);
	minimap = new Entity();
	minimap->AttachComponent<SpriteRenderer>()->SetTexture("Graphic/Minimap.png");
	minimap->scale = Vec2F(1, 1);
	button = new Button("Text");
	AddChild(button);
	SetMinimap(true);
	SetUI();
	ChangeBuilding(Ally.at(3), Tag::TagABarrack);
	ChangeBuilding(Ally.at(4), Tag::TagWBarrack);
	ChangeBuilding(Ally.at(5), Tag::TagGBarrack);
	ButtonSetting(UI);
	RG2SoundManager->Load(SoundID::SIDBgm, "Audio/1.ogg");
	RG2SoundManager->Play(SoundID::SIDBgm, true, false);
}


void Scene1::OnUpdate() { 
	Scene::OnUpdate();
	Faith += (float(10) / float(60));
	FaithView->SetText(faith + to_string(int(Faith)));
	if (GetForegroundWindow() == RG2Window->GetHwnd()) {
		RG2Window->SetClipCursor(true);
		CameraMove(RG2Input->GetMousePos());
	}
	else
	RG2Window->SetClipCursor(false);

}

Scene1::~Scene1()
{
}

void Scene1::CameraMove(const Vec2F Pos) {
	if(!this->camera.target){
	if (Pos.x <= 0)
		camera.SetPos(Vec2F(35, 0));
	if (Pos.y <= 0)
		camera.SetPos(Vec2F(0, 35));
	if (Pos.x >= 1279)
		camera.SetPos(Vec2F(-35, 0));
	if (Pos.y >= 719)
		camera.SetPos(Vec2F(0, -35));
	}
}

void Scene1::SetMinimap(bool set) {
	if (set) {
		AddChild(minimap);
		camera.SetMinimap(minimap->pos);
		SetMouseListener(minimap);
	}
	else{
		camera.SetMinimap(minimap->pos);
		RemoveChild(minimap);
		
	}
}

void Scene1::SetMouseListener(Entity* entity) {
		entity->AttachComponent<MouseListener>()->SetOnClickListener([=]() {

			if (entity->GetComponent<MouseListener>()->IsClicked == false) {
				for (auto i : this->GetChild())
					if (i->GetComponent<MouseListener>() == nullptr)
						continue;
					else if (i == entity) i->GetComponent<MouseListener>()->IsClicked = true;
					else i->GetComponent<MouseListener>()->IsClicked = false;
					switch (entity->GetTag()) {
					case Tag::TagTower:TowerButton(UI); break;
					case Tag::TagABarrack:ABarruckButton(UI); break;
					case Tag::TagGBarrack:GBarruckButton(UI); break;
					case Tag::TagWBarrack:WBarruckButton(UI); break;
					default: DefaultButton(UI);
					}
			}
			else {
				entity->GetComponent<MouseListener>()->IsClicked = false;
			}
		});
}

void Scene1::SetAllyBase() {
	int AllySize = 0;
	AllySize = Ally.size();
	for (auto i = 0; i < AllySize; i++) {
		Ally.at(i) = new Entity();
		AddChild(Ally.at(i));
		if (i == 0) {
			Ally.at(0)->AttachComponent<SpriteRenderer>()->SetTexture("Graphic/Nexus.png");
		}
		else if (i > 0) {
			Ally.at(i)->AttachComponent<SpriteRenderer>()->SetTexture("Graphic/Tower.png");
		}
		Ally.at(i)->AttachComponent<RigidBody>(false, false)->isFixed = true;
		Ally.at(i)->AttachComponent<AABBCollider>()->SetRect(Ally.at(i)->GetComponent<SpriteRenderer>()->visibleRect)->SetOnCollisionListener([](Entity* other) {});
		SetMouseListener(Ally.at(i));
	}
	Ally.at(1)->SetPos(Vec2F(700, 50))->SetTag(Tag::TagTower);
	Ally.at(2)->SetPos(Vec2F(700, 500))->SetTag(Tag::TagTower);
	Ally.at(3)->SetPos(Vec2F(700, -400))->SetTag(Tag::TagTower);
	Ally.at(4)->SetPos(Vec2F(1500, 500))->SetTag(Tag::TagTower);
	Ally.at(5)->SetPos(Vec2F(1500, -400))->SetTag(Tag::TagTower);
}

void Scene1::SetEnemyBase() {
	int EnemySize = 0;
	EnemySize = Enemy.size();
	for (auto i = 0; i < EnemySize; i++) {
		Enemy.at(i) = new Entity();
		AddChild(Enemy.at(i));
		if (i == 0) {
			Enemy.at(0)->AttachComponent<SpriteRenderer>()->SetTexture("Graphic/Nexus.png");
		}
		else if (i > 0) {
			Enemy.at(i)->AttachComponent<SpriteRenderer>()->SetTexture("Graphic/Tower.png");
		}
		Enemy.at(i)->AttachComponent<RigidBody>(false, false)->isFixed = true;
		Enemy.at(i)->AttachComponent<AABBCollider>()->SetRect(Enemy.at(i)->GetComponent<SpriteRenderer>()->visibleRect)->SetOnCollisionListener([](Entity* other) {});
		SetMouseListener(Enemy.at(i));
	}
	Enemy.at(0)->SetPos(Vec2F(6500, 0));
	Enemy.at(2)->SetPos(Vec2F(5000, 500))->SetTag(Tag::TagTower);
	Enemy.at(3)->SetPos(Vec2F(5000, -400))->SetTag(Tag::TagTower);
	Enemy.at(1)->SetPos(Vec2F(5800, 50))->SetTag(Tag::TagTower);
	Enemy.at(4)->SetPos(Vec2F(5800, 500))->SetTag(Tag::TagTower);
	Enemy.at(5)->SetPos(Vec2F(5800, -400))->SetTag(Tag::TagTower);
}

void Scene1::SetUI() {
	for (int i = 0; i < UI.size(); i++) {
		UI.at(i) = new Button(" ");
		camera.SetUI(UI.at(i)->pos);
		AddChild(UI.at(i));
	}
}

void Scene1::TowerButton(vector<Button*> UI) {
	UI.at(0)->SetText(string("건물 업그레이드"));
	UI.at(0)->SetTag(Tag::TagUpgrade);

	UI.at(1)->SetText(string("철거"));
	UI.at(1)->SetTag(Tag::TagPDown);

	UI.at(2)->SetText(string("건물 교체"));
	UI.at(2)->SetTag(Tag::TagChangeBuilding);

	UI.at(3)->SetText(string(" "));
	UI.at(3)->SetTag(Tag::None);

	UI.at(4)->SetText(string(" "));
	UI.at(4)->SetTag(Tag::None);
}

void Scene1::ABarruckButton(vector<Button*> UI) {
	UI.at(0)->SetText(string("궁수 생성"));
	UI.at(0)->SetTag(Tag::TagAddArcher);

	UI.at(1)->SetText(string("건물 업그레이드"));
	UI.at(1)->SetTag(Tag::TagUpgrade);

	UI.at(2)->SetText(string("철거"));
	UI.at(2)->SetTag(Tag::TagPDown);

	UI.at(3)->SetText(string("건물 교체"));
	UI.at(3)->SetTag(Tag::TagChangeBuilding);

	UI.at(4)->SetText(string("공격 건물 지정"));
	UI.at(4)->SetTag(Tag::TagAttackTarget);
}
void Scene1::GBarruckButton(vector<Button*> UI) {
	UI.at(0)->SetText(string("가디언 생성"));
	UI.at(0)->SetTag(Tag::TagAddWarrior);

	UI.at(1)->SetText(string("건물 업그레이드"));
	UI.at(1)->SetTag(Tag::TagUpgrade);

	UI.at(2)->SetText(string("철거"));
	UI.at(2)->SetTag(Tag::TagPDown);

	UI.at(3)->SetText(string("건물 교체"));
	UI.at(3)->SetTag(Tag::TagUpgrade);

	UI.at(4)->SetText(string("공격 건물 지정"));
	UI.at(2)->SetTag(Tag::TagAttackTarget);

}
void Scene1::WBarruckButton(vector<Button*> UI) {
	UI.at(0)->SetText(string("병사 생성"));
	UI.at(0)->SetTag(Tag::TagAddWarrior);

	UI.at(1)->SetText(string("건물 업그레이드"));
	UI.at(1)->SetTag(Tag::TagUpgrade);

	UI.at(2)->SetText(string("철거"));
	UI.at(2)->SetTag(Tag::TagPDown);

	UI.at(3)->SetText(string("건물 교체"));
	UI.at(3)->SetTag(Tag::TagChangeBuilding);

	UI.at(4)->SetText(string("공격 건물 지정"));
	UI.at(4)->SetTag(Tag::TagAttackTarget);
}

void Scene1::TempleButton(vector<Button*> UI) {
}

void Scene1::DefaultButton(vector<Button*> UI) {
}

void Scene1::ChangeBuilding(Entity* building , Tag tag) {
	switch (tag)
	{
	case TagTower:building->GetComponent<SpriteRenderer>()->SetTexture("Graphic/Tower.png");
		building->SetTag(Tag::TagTower);
		break;
	case TagWBarrack:building->GetComponent<SpriteRenderer>()->SetTexture("Graphic/WarriorBarruck.png");
		building->SetTag(Tag::TagWBarrack);
		break;
	case TagABarrack:building->GetComponent<SpriteRenderer>()->SetTexture("Graphic/ArcherBarruck.png");
		building->SetTag(Tag::TagABarrack);
		break;
	case TagGBarrack:building->GetComponent<SpriteRenderer>()->SetTexture("Graphic/GuardionBarruck.png");
		building->SetTag(Tag::TagGBarrack);
		break;
	default:
		break;
	}
	building->SetHp(tag);
}

void Scene1::UpgradeBuilding(Tag tag , int Upgrade) {
	if (tag == Tag::TagABarrack && (Faith - 750 * Upgrade) > 0) {
		cout << Upgrade << endl;
		Faith -= 750 * Upgrade;
		Upgrade++;
		cout << Upgrade << endl;
	}
}

void Scene1::UpgradeUnit(Tag tag, int Upgrade) {

}

void Scene1::ButtonSetting(vector<Button*> UI) {
	for(int i = 0 ; i< UI.size() ; i++)
	UI.at(i)->AttachComponent<MouseListener>()->SetOnClickListener([=]() {
		for (auto j : Ally)
			if (j->GetComponent<MouseListener>()->IsClicked) {
				switch (UI.at(i)->GetTag())
				{
				case TagUpgrade:UpgradeBuilding(j->GetTag(), j->BuildingUpgrade);
					break;
				case TagPDown:
					break;
				case TagAddWarrior:
					break;
				case TagAddArcher:
					break;
				case TagAddGurdion:
					break;
				case TagAttackTarget:
					break;
				case TagChangeBuilding:
					break;
				default:
					break; 
				}
				break;
			}
	});
}
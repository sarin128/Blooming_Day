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
	SetMainCursor();
	Ally.resize(3);
	Enemy.resize(3);
	Neutrality.resize(4);
	UI.resize(5);
	Player = new testEntity();
	AddChild(Player);

	SetAllyBase();
	SetEnemyBase();
	SetNeutralityBase();
	camera.UnFollow();
	camera.SetSmoothFollowEnabled(true);
	camera.zoom = 1.0f;

	Faith = 3000;
	FaithView = new Button(" ");
	HpView = new Button(" ");
	UpgradeView = new Button(" ");
	camera.SetFaithScore(FaithView->pos);
	camera.SetHpViewer(HpView->pos);
	camera.SetUpgradeViewer(UpgradeView->pos);
	AddChild(UpgradeView);
	AddChild(FaithView);
	AddChild(HpView);
	minimap = new Entity();
	minimap->AttachComponent<SpriteRenderer>()->SetTexture("Graphic/Minimap.png");
	minimap->scale = Vec2F(1, 1);
	SetMinimap(false);
	SetUI();
	ButtonSetting(UI);
	RG2SoundManager->Load(SoundID::SIDBgm, "Audio/MainBattle.ogg");
	RG2SoundManager->Play(SoundID::SIDBgm, true, false);
	Player->PathFinding(Enemy.at(0)->GetComponent<AABBCollider>()->GetRectWithPos().GetCenter());

}


void Scene1::OnUpdate() { 
	Scene::OnUpdate();
	Faith += (float(10) / float(60));
	for (auto building : Ally) {
		if (building->GetTag() == Tag::TagTemple)
			Faith += (float(5 * building->BuildingUpgrade) / float(60));
	}
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
			if (CursorPath == LPCWSTR(L"Graphic/Mecha-Precision.ANI") && RG2Input->GetMouseState(MouseCode::MOUSE_LBUTTON) == KeyState::KEYSTATE_ENTER) {
				for (auto i : this->GetChild()) {
					if (i->GetComponent<MouseListener>() == nullptr)
						continue;
					if (i->GetComponent<MouseListener>()->IsClicked == true) {
						i->Path = entity->GetComponent<AABBCollider>()->GetRectWithPos().GetCenter();
						break;
					}
				}
				SetMainCursor();
				return;
			}
			SetMainCursor();
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
					case Tag::TagBuildingGround: DefaultButton(UI); break;
					case Tag::TagNexus: NexusButton(UI); break;
					default: break;
					}
					HpView->SetText(targetHp + to_string(int(entity->hp)));
					UpgradeView->SetText(upgrade + to_string(int(entity->BuildingUpgrade)));
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
		Ally.at(i)->AttachComponent<AABBCollider>()->SetRect(Ally.at(i)->GetComponent<SpriteRenderer>()->visibleRect);
		SetMouseListener(Ally.at(i));
	}
	Ally.at(0)->SetTag(Tag::TagNexus)->SetFlag(Flag::Ally);
	Ally.at(0)->SetHp(Ally.at(0)->GetTag());
	Ally.at(0)->SetFlag(Flag::Ally);
	Ally.at(1)->SetPos(Vec2F(700, -600))->SetTag(Tag::TagTower);
	Ally.at(1)->SetHp(Ally.at(1)->GetTag());
	Ally.at(1)->SetFlag(Flag::Ally);
	Ally.at(2)->SetPos(Vec2F(700, 600))->SetTag(Tag::TagTower)->SetFlag(Flag::Ally);
	Ally.at(2)->SetFlag(Flag::Ally);
	Ally.at(2)->SetHp(Ally.at(1)->GetTag());
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
	Enemy.at(0)->SetPos(Vec2F(6100, 0))->SetTag(Tag::TagNexus)->SetFlag(Flag::Enemy);
	Enemy.at(0)->SetHp(Enemy.at(0)->GetTag());
	Enemy.at(1)->SetPos(Vec2F(5400, -600))->SetTag(Tag::TagTower)->SetFlag(Flag::Enemy);
	Enemy.at(1)->SetHp(Enemy.at(1)->GetTag());
	Enemy.at(2)->SetPos(Vec2F(5400, 600))->SetTag(Tag::TagTower)->SetFlag(Flag::Enemy);
	Enemy.at(2)->SetHp(Enemy.at(2)->GetTag());
}

void Scene1::SetNeutralityBase() {
	int NeutralitySize = 0;
	NeutralitySize = Neutrality.size();
	for (auto i = 0; i < NeutralitySize; i++) {
		Neutrality.at(i) = new Entity();
		AddChild(Neutrality.at(i));
		Neutrality.at(i)->BuildingUpgrade = 0;
		Neutrality.at(i)->AttachComponent<SpriteRenderer>()->SetTexture("Graphic/BuildingGround.png");
		Neutrality.at(i)->AttachComponent<RigidBody>(false, false)->isFixed = true;
		Neutrality.at(i)->AttachComponent<AABBCollider>()->SetRect(Neutrality.at(i)->GetComponent<SpriteRenderer>()->visibleRect)->SetOnCollisionListener([](Entity* other) {});
		SetMouseListener(Neutrality.at(i));
	}
	Neutrality.at(0)->SetPos(Vec2F(2150, 0))->SetTag(Tag::TagBuildingGround)->SetFlag(Flag::Neutrality);
	Neutrality.at(1)->SetPos(Vec2F(2750, 1500))->SetTag(Tag::TagBuildingGround)->SetFlag(Flag::Neutrality);
	Neutrality.at(2)->SetPos(Vec2F(3550, -1500))->SetTag(Tag::TagBuildingGround)->SetFlag(Flag::Neutrality);
	Neutrality.at(3)->SetPos(Vec2F(4150, 0))->SetTag(Tag::TagBuildingGround)->SetFlag(Flag::Neutrality);
}

void Scene1::SetUI() {
	int UIsize = UI.size();
	for (int i = 0; i< UIsize; i++) {
		UI.at(i) = new Button(" ");
		UI.at(i)->AttachComponent<AABBCollider>()->SetRect(UI.at(i)->GetComponent<SpriteRenderer>()->visibleRect);
		UI.at(i)->GetComponent<AABBCollider>()->SetEnabled(false);
		camera.SetUI(UI.at(i)->pos);
		AddChild(UI.at(i));
	}
}

void Scene1::TowerButton(vector<Button*> UI) {
	UI.at(0)->SetText(string("업그레이드"));
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

	UI.at(1)->SetText(string("업그레이드"));
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
	UI.at(0)->SetTag(Tag::TagAddGuardian);

	UI.at(1)->SetText(string("업그레이드"));
	UI.at(1)->SetTag(Tag::TagUpgrade);

	UI.at(2)->SetText(string("철거"));
	UI.at(2)->SetTag(Tag::TagPDown);

	UI.at(3)->SetText(string("건물 교체"));
	UI.at(3)->SetTag(Tag::TagChangeBuilding);

	UI.at(4)->SetText(string("공격 건물 지정"));
	UI.at(4)->SetTag(Tag::TagAttackTarget);
}

void Scene1::WBarruckButton(vector<Button*> UI) {
	UI.at(0)->SetText(string("병사 생성"));
	UI.at(0)->SetTag(Tag::TagAddWarrior);

	UI.at(1)->SetText(string("업그레이드"));
	UI.at(1)->SetTag(Tag::TagUpgrade);

	UI.at(2)->SetText(string("철거"));
	UI.at(2)->SetTag(Tag::TagPDown);

	UI.at(3)->SetText(string("건물 교체"));
	UI.at(3)->SetTag(Tag::TagChangeBuilding);

	UI.at(4)->SetText(string("공격 건물 지정"));
	UI.at(4)->SetTag(Tag::TagAttackTarget);
}

void Scene1::TempleButton(vector<Button*> UI) {
	UI.at(0)->SetText(string("건물 업그레이드"));
	UI.at(0)->SetTag(Tag::TagUpgrade);

	UI.at(1)->SetText(string("철거"));
	UI.at(1)->SetTag(Tag::TagPDown);

	UI.at(2)->SetText(string(" "));
	UI.at(2)->SetTag(Tag::None);

	UI.at(3)->SetText(string(" "));
	UI.at(3)->SetTag(Tag::None);

	UI.at(4)->SetText(string(" "));
	UI.at(4)->SetTag(Tag::None);
}

void Scene1::DefaultButton(vector<Button*> UI) {
	UI.at(0)->SetText(string("건물 건설"));
	UI.at(0)->SetTag(Tag::TagChangeBuilding);

	UI.at(1)->SetText(string("건물 건설"));
	UI.at(1)->SetTag(Tag::TagChangeBuilding);

	UI.at(2)->SetText(string("건물 건설"));
	UI.at(2)->SetTag(Tag::TagChangeBuilding);

	UI.at(3)->SetText(string("건물 건설"));
	UI.at(3)->SetTag(Tag::TagChangeBuilding);

	UI.at(4)->SetText(string("건물 건설"));
	UI.at(4)->SetTag(Tag::TagChangeBuilding);
}

void Scene1::ChangeBuidingButtion(vector<Button*>UI) {
	UI.at(0)->SetText(string("병사 막사"));
	UI.at(0)->SetTag(Tag::TagCreateWBarrack);

	UI.at(1)->SetText(string("가디언 막사"));
	UI.at(1)->SetTag(Tag::TagCreateGBarrack);

	UI.at(2)->SetText(string("궁수 막사"));
	UI.at(2)->SetTag(Tag::TagCreateABarrack);

	UI.at(3)->SetText(string("타워"));
	UI.at(3)->SetTag(Tag::TagCreateTower);

	UI.at(4)->SetText(string("사원"));
	UI.at(4)->SetTag(Tag::TagCreateTemple);
}

void Scene1::ChangeBuilding(Entity* building , Tag tag) {
	switch (tag)
	{
	case TagCreateTemple:building->GetComponent<SpriteRenderer>()->SetTexture("Graphic/Temple.png");
		building->SetTag(Tag::TagTemple);
		break;
	case TagCreateTower:building->GetComponent<SpriteRenderer>()->SetTexture("Graphic/Tower.png");
		building->SetTag(Tag::TagTower);
		break;
	case TagCreateWBarrack:building->GetComponent<SpriteRenderer>()->SetTexture("Graphic/WarriorBarruck.png");
		building->SetTag(Tag::TagWBarrack);
		break;
	case TagCreateABarrack:building->GetComponent<SpriteRenderer>()->SetTexture("Graphic/ArcherBarruck.png");
		building->SetTag(Tag::TagABarrack);
		break;
	case TagCreateGBarrack:building->GetComponent<SpriteRenderer>()->SetTexture("Graphic/GuardionBarruck.png");
		building->SetTag(Tag::TagGBarrack);
		break;
	default:
		break; 
	}
	building->BuildingUpgrade = 1;
	building->GetComponent<AABBCollider>()->SetRect(building->GetComponent<SpriteRenderer>()->visibleRect);
	building->SetHp(tag);
}

void Scene1::UpgradeBuilding(Tag tag , Entity* Target) {
	if (Faith - 750 * Target->BuildingUpgrade > 0) {
		switch (tag)
		{
		case TagTower:
		case TagTemple:
			Faith -= 500 * Target->BuildingUpgrade;
			break;
		case TagWBarrack:
		case TagABarrack:
		case TagGBarrack:
			Faith -= 750 * Target->BuildingUpgrade;
			break;
		default:
			break;
		}
		Target->BuildingUpgrade++;
		cout << Target->BuildingUpgrade << endl;
	}

	else
		return;
}

void Scene1::PutDownBuilding(Entity* Building, int Upgrade) {
	switch (Building->GetTag())
	{
	case TagNexus: return;
		break;
	case TagTower:
		Faith += 250 * Upgrade;
		break;
	case TagTemple:
		Faith += 200 * Upgrade;
		break;
	case TagWBarrack:
	case TagABarrack:
	case TagGBarrack:
		Faith += 300 * Upgrade;
		break;
	default:
		break;
	}
	Building->BuildingUpgrade = 0;
	Building->GetComponent<SpriteRenderer>()->SetTexture("Graphic/BuildingGround.png");
	Building->GetComponent<AABBCollider>()->SetRect(Building->GetComponent<SpriteRenderer>()->visibleRect);
	Building->SetTag(Tag::TagBuildingGround)->SetHp(Tag::TagBuildingGround);
}

void Scene1::ButtonSetting(vector<Button*> UI) {
	int UIsize = UI.size();
	for(int i = 0 ; i< UIsize; i++)
	UI.at(i)->AttachComponent<MouseListener>()->SetOnClickListener([=]() {
		for (auto j : this->GetChild())
			if (j->GetFlag() == Flag::Ally && j->GetComponent<MouseListener>()->IsClicked) {
				switch (UI.at(i)->GetTag())
				{
				case TagUpgrade:UpgradeBuilding(j->GetTag(), j);
					break;
				case TagPDown:PutDownBuilding(j, j->BuildingUpgrade);
					break;
				case TagAddWarrior:
				case TagAddArcher:
				case TagAddGuardian:AddUnit(UI.at(i)->GetTag(), j);
					break;
				case TagAttackTarget:SetTargetCursor();
					break;
				case TagChangeBuilding: ChangeBuidingButtion(UI);
					break;
				case TagCreateWBarrack:if (j->GetTag() == Tag::TagWBarrack) return;
					CreateBuilding(j, UI.at(i)->GetTag());
					break;
				case TagCreateABarrack:if (j->GetTag() == Tag::TagABarrack) return;
					CreateBuilding(j, UI.at(i)->GetTag());
					break;
				case TagCreateGBarrack:if (j->GetTag() == Tag::TagGBarrack) return;
					CreateBuilding(j, UI.at(i)->GetTag());
					break;
				case TagCreateTower: if (j->GetTag() == Tag::TagTower) return;
					CreateBuilding(j, UI.at(i)->GetTag());
					break;
				case TagCreateTemple: if (j->GetTag() == Tag::TagTemple) return;
					CreateBuilding(j, UI.at(i)->GetTag());
					break;
				default:
					break; 
				}
				UpgradeView->SetText(upgrade + to_string(int(j->BuildingUpgrade)));
				break;
			}
	});
}

void Scene1::CreateBuilding(Entity* Building, Tag tag) {
		switch (tag) {
		case TagCreateWBarrack:
		case TagCreateABarrack:
		case TagCreateGBarrack:
			if (Faith - Price::Barrack < 0) return;
			Faith -= Price::Barrack;
			break;
		case TagCreateTemple:
			if (Faith - Price::Temple < 0) return;
			Faith -= Price::Temple;
			break;
		case TagCreateTower:if (Faith - Price::Tower < 0) return;
			Faith -= Price::Tower;
			break;
		default:break;
		}
		PutDownBuilding(Building, Building->BuildingUpgrade);
		ChangeBuilding(Building, tag);
}

void Scene1::SetMainCursor() {
	CursorPath = (L"Graphic/Mecha-Normal_1.ANI");
	SetCursorImage = LoadCursorFromFile(CursorPath);
	SetCursor(SetCursorImage);
}

void Scene1::SetTargetCursor() {
	CursorPath = (L"Graphic/Mecha-Precision.ANI");
	SetCursorImage = LoadCursorFromFile(CursorPath);
	SetCursor(SetCursorImage);
}

void Scene1::AddUnit(Tag tag, Entity* Building) {
	Entity* Unit = new testEntity();
	AddChild(Unit);
	Unit->PathFinding(Building->Path);
	Unit->SetPos(Vec2F(Building->GetComponent<AABBCollider>()->GetRectWithPos().GetCenter().x - 100, Building->GetComponent<AABBCollider>()->GetRectWithPos().GetCenter().y));
	switch (tag) {
	case Tag::TagAddArcher: Unit->SetTag(Tag::TagArcher);
	case Tag::TagAddWarrior: Unit->SetTag(Tag::TagWarrior);
	case Tag::TagAddGuardian: Unit->SetTag(Tag::TagGuardian);
	}
	AllyUnit.push_back(Unit);
}

void Scene1::NexusButton(vector<Button*>UI) {
	UI.at(0)->SetText(string(" "));
	UI.at(0)->SetTag(Tag::None);

	UI.at(1)->SetText(string(" "));
	UI.at(1)->SetTag(Tag::None);

	UI.at(2)->SetText(string(" "));
	UI.at(2)->SetTag(Tag::None);

	UI.at(3)->SetText(string(" "));
	UI.at(3)->SetTag(Tag::None);

	UI.at(4)->SetText(string(" "));
	UI.at(4)->SetTag(Tag::None);
}
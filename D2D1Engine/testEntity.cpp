#include "pch.h"
#include "testEntity.h"
#include "SpriteRenderer.h"
#include "RigidBody.h"
#include "AABBCollider.h"

testEntity::testEntity()
{
	AttachComponent<SpriteRenderer>()->SetTexture("Graphic/player.png");
	AttachComponent<AABBCollider>()->SetRect(GetComponent<SpriteRenderer>()->visibleRect)->SetOnCollisionListener([=](Entity* other) {
		Vec2F TargetPos = other->GetComponent<AABBCollider>()->GetRectWithPos().GetCenter();
		Vec2F UnitPos = GetComponent<AABBCollider>()->GetRectWithPos().GetCenter();
		if (TargetPos.y > UnitPos.y) {
			this->pos.y -= 10;
			return;
		}
		else {
			this->pos.y += 10;
			return;
		}
	});
	AttachComponent<RigidBody>(false,false);

	this->SetPos(Vec2F(100, 50));
}
void testEntity::OnUpdate() {
	if (Path) {
		this->pos.x += 3;
		if (abs(Path.y - this->pos.y) < 5)
			this->pos.y += 0;
		else if (Path.y > this->pos.y)
			this->pos.y += 5;
		else
			this->pos.y -= 5;
	}
	
	if (RG2Input->GetKeyState(KeyCode::KEY_ESCAPE) == KeyState::KEYSTATE_ENTER) {
		DestroyWindow(RG2Window->GetHwnd());
	}
	if (RG2Input->GetKeyState(KeyCode::KEY_LEFT) == KeyState::KEYSTATE_STAY) {
		this->pos.x -= 15;
	}
	if (RG2Input->GetKeyState(KeyCode::KEY_RIGHT) == KeyState::KEYSTATE_STAY) {
		this->pos.x += 15;
	}
	if (RG2Input->GetKeyState(KeyCode::KEY_UP) == KeyState::KEYSTATE_STAY) {
		this->pos.y -= 15;
	}
	if (RG2Input->GetKeyState(KeyCode::KEY_DOWN) == KeyState::KEYSTATE_STAY) {
		this->pos.y += 15;
	}

}

testEntity::~testEntity()
{
}
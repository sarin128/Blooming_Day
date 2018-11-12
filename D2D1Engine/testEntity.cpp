#include "pch.h"
#include "testEntity.h"
#include "SpriteRenderer.h"
#include "RigidBody.h"
#include "AABBCollider.h"

testEntity::testEntity()
{
	AttachComponent<SpriteRenderer>()->SetTexture("Graphic/player.png");
	AttachComponent<AABBCollider>()->SetRect(GetComponent<SpriteRenderer>()->visibleRect)->SetOnCollisionListener([](Entity* other) {});
	AttachComponent<RigidBody>(false,false);

	this->SetPos(Vec2F(50, 50));
}
void testEntity::OnUpdate() {
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

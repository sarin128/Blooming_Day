#pragma once
#include "Component.h"
//중력가속도(1초당)
#define AG (9.8f)
//중력가속도(1프레임당)
#define AG_PER_DT (AG*DT)
class RigidBody :
	public Component
{
public:
	float gravity;
	bool isFixed;
	bool isGravityEnabled;
	bool isTouchingBottom;
	bool isTouchingTop;
	bool isTouchingRight;
	bool isTouchingLeft;

public:
	RigidBody(Entity* owner, bool isFixed = false, bool isGravityEnabled = true);
	virtual ~RigidBody() override;

public:
	virtual std::type_index GetID(void) override { return typeid(RigidBody); }
	static std::type_index GetFamilyID(void) { return typeid(RigidBody); }

	void OnUpdate(void) override;
};


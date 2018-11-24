#pragma once
#include "Entity.h"
enum UnitType {
	Archer,
	Guardion,
	Warrior,
};
class testEntity :
	public Entity
{
public:
	testEntity();
	~testEntity();
	void OnUpdate();
};


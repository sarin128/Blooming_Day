#pragma once
#include "Entity.h"
class GameScene;
class UI :
	public Entity
{
public:
	Entity* scoreText;
	Entity* playerHpText;

	GameScene* context;
public:
	UI(GameScene* context);
	~UI();

	void OnUpdate() override;
};


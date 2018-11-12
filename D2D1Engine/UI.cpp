#include "pch.h"
#include "UI.h"
#include"TextRenderer.h"
#include"GameScene.h"
#include"CActor.h"
#include"CTextEffect2.h"
UI::UI(GameScene* context)
	:context(context)
{
	AddChild(scoreText = new Entity());
	scoreText->AttachComponent<TextRenderer>("Andy", 75, 3.f,ColorType::CadetBlue, ColorType::AntiqueWhite, "123456789");
	AddChild(playerHpText = new Entity());
	playerHpText->AttachComponent<TextRenderer>("Andy", 32, 0.f, ColorType::AliceBlue,ColorType::AliceBlue, "Default");

	scoreText->pos = Vec2F(50, 50);
	playerHpText->pos = Vec2F(50,130);


	auto e = new Entity();
	e->AttachComponent<TextRenderer>("Andy", 80, 3.f, ColorType::DeepSkyBlue);
	e->AttachComponent<CTextEffect2>("Hunt your enemies!!!\nSurvive as long as possible!!!");
	AddChild(e);
}

UI::~UI()
{
}

void UI::OnUpdate()
{
	Entity::OnUpdate();
	scoreText->GetComponent<TextRenderer>()->SetText(std::to_string(context->score));
	playerHpText->GetComponent<TextRenderer>()->SetText(std::to_string((int)context->FindEntity(TagPlayer)->GetComponent<CActor>()->hp));
}

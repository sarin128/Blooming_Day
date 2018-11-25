#include "pch.h"
#include "Button.h"
#include "AABBCollider.h"
#include "SpriteRenderer.h"
#include "TextRenderer.h"
#include "MouseListener.h"
#include "KeyListener.h"
#include "Window.h"
//
//Entity *owner, const std::string& fontName = "¸¼Àº °íµñ", int size = 12,
//float outlineWidth = 0.f,
//const Color& fill = Color(1.f, 1.f, 1.f, 1.f),
//const Color& outline = Color(0.f, 0.f, 0.f, 0.f), const std::string& text = ""

Button::Button(const std::string& t)
{
	AddChild(text = new Entity());
	sr = AttachComponent<SpriteRenderer>();
	GetComponent<SpriteRenderer>()->SetTexture("Graphic/Button.png",true);
	tr = text->AttachComponent<TextRenderer>(
		"Comic Sans MS", 14, 5.f, Color(0.f, 0.3f, 0.7f, 1.f),
		Color(1.f,1.f,1.f,1.f), t);
		tr->SetEnabled(true);
		text->pos.y = 15;

}
void Button::SetText(string& t) {
	tr->SetText(t);
}
void Button::AddText(string&t) {
	string text = tr->GetText() + t;
}

void Button::SetTexture(const string t) {
	sr->SetTexture("Graphic/button.png");
}

Button::~Button()
{

}


void Button::SetButtonClickListener() {

}
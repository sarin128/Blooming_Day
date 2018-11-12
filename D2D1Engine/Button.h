#pragma once
#include "Entity.h"
#include "SpriteRenderer.h"
class Button :
	public Entity
{
public:
	Entity* text;
public:
	Button(const std::string& text);
	~Button();
	void SetOnClickListener(std::function<void()> callback);
	TextRenderer* tr;
	SpriteRenderer* sr;
	bool isClicked = false;
	void SetText(string& t);
	void AddText(string& t);
	void SetTexture(const string t);
	void SetButtonClickListener();
};

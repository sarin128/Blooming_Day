#pragma once
#include "Scene.h"
#include "Button.h"
class Login :
	public Scene
{
public:
	Login();
	~Login();
	Button* Id;
	Button* Password;
	KeyCode key;
	void SetMouseListener(Button* entity);
	void OnUpdate();
};


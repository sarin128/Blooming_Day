#include "pch.h"
#include "Login.h"
#include "app.h"
#include "Scene1.h"
#include "MouseListener.h"
#include "Camera.h"
#include "Window.h"
Login::Login()
{
	Id = new Button("Enter the Id");
	Password = new Button("Enter the Paswword");
	Password->SetPos(Vec2F(50, 50));
	camera.SetPos(Vec2F(0, 0));
	SetMouseListener(Id);
	SetMouseListener(Password);
	AddChild(Id);
	AddChild(Password);
}


Login::~Login()

{
}

void Login::OnUpdate() {
	if (Id->GetComponent<MouseListener>()->IsClicked) {
		HWND hwnd = CreateWindow(TEXT("a"), TEXT("aasd"), WS_VISIBLE | WS_CHILD | WS_BORDER, 50, 50, 200, 25,RG2Window->GetHwnd(),(HMENU)NULL,NULL,NULL);

	}
}

void Login::SetMouseListener(Button* entity) {
	entity->AttachComponent<MouseListener>()->SetOnClickListener([=]() {
		if (entity->GetComponent<MouseListener>()->IsClicked == false) {
			for (auto i : this->GetChild())
				if (i->GetComponent<MouseListener>() == nullptr)
					continue;
				else if (i == entity) i->GetComponent<MouseListener>()->IsClicked = true;
				else i->GetComponent<MouseListener>()->IsClicked = false;
		}
		else {
			entity->GetComponent<MouseListener>()->IsClicked = false;
		}
	});
}
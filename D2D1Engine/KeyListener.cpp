#include "pch.h"
#include "KeyListener.h"
#include"Entity.h"
#include"Collider.h"
#include"AABBCollider.h"
#include"Input.h"
#include"App.h"
KeyListener::KeyListener(Entity * owner, KeyCode key)
	:Component(owner),key(key)
{
	onEnter_ = []() {};
	onStay_ = []() {};
	onExit_ = []() {};
}

KeyListener::~KeyListener()
{
}

KeyListener* KeyListener::SetOnEnterListener(std::function<void()> callback)
{
	onEnter_ = callback;
	return this;
}

KeyListener* KeyListener::SetOnStayListener(std::function<void()> callback)
{
	onStay_ = callback;
	return this;
}

KeyListener* KeyListener::SetOnExitListener(std::function<void()> callback)
{
	onExit_ = callback;
	return this;
}

KeyCode KeyListener::GetKey() {
	return key;
}

void KeyListener::OnUpdate()
{
	if(RG2Input->GetKeyState(key) == KEYSTATE_ENTER)
	{
		onEnter_();
	}
	else if(RG2Input->GetKeyState(key) == KEYSTATE_STAY)
	{
		onStay_();
	}
	else if (RG2Input->GetKeyState(key) == KEYSTATE_EXIT)
	{
		onExit_();
	}
}



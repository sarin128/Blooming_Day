#pragma once
#include "Component.h"
class KeyListener :
	public Component
{
private:
	std::function<void()> onEnter_;
	std::function<void()> onStay_;
	std::function<void()> onExit_;
public:
	//반응할 키
	KeyCode key;
public:
	KeyListener(Entity *owner, KeyCode key);
	~KeyListener() override;

	KeyListener* SetOnEnterListener(std::function<void()> callback);
	KeyListener* SetOnStayListener(std::function<void()> callback);
	KeyListener* SetOnExitListener(std::function<void()> callback);
	KeyCode GetKey();
	std::type_index GetID(void) override { return typeid(KeyListener); }
	static std::type_index GetFamilyID(void) { return typeid(KeyListener); }

	void OnUpdate() override;


	

};

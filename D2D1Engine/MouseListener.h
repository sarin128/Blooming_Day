#pragma once
#include "Component.h"
class MouseListener :
	public Component
{
private:
	std::function<void()> onClick_;
	std::function<void()> onEnter_;
	std::function<void()> onOver_;
	std::function<void()> onExit_;
	Point2F lastMousePos_;
public:
	MouseListener(Entity *owner);
	~MouseListener() override;
	bool IsClicked = false;
	bool IsButtonClicked = false;
	MouseListener* SetOnClickListener(std::function<void()> callback);
	MouseListener* SetOnEnterListener(std::function<void()> callback);
	MouseListener* SetOnOverListener(std::function<void()> callback);
	MouseListener* SetOnExitListener(std::function<void()> callback);

	void Reset(void) override;
	std::type_index GetID(void) override { return typeid(MouseListener); }
	static std::type_index GetFamilyID(void) { return typeid(MouseListener); }

	void OnUpdate() override;

};


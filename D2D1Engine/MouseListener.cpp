#include "pch.h"
#include "MouseListener.h"
#include"Entity.h"
#include"Collider.h"
#include"AABBCollider.h"
#include"Input.h"
#include"App.h"
MouseListener::MouseListener(Entity * owner)
	:Component(owner),
	lastMousePos_(0,0)
{
	onClick_ = []() {};
	onEnter_ = []() {};
	onOver_ = []() {};
	onExit_ = []() {};
}

MouseListener::~MouseListener()
{
}

MouseListener* MouseListener::SetOnClickListener(std::function<void()> callback)
{
	onClick_ = callback;
	return this;
}

MouseListener* MouseListener::SetOnEnterListener(std::function<void()> callback)
{
	onEnter_ = callback;
	return this;
}

MouseListener* MouseListener::SetOnOverListener(std::function<void()> callback)
{
	onOver_ = callback;
	return this;
}

MouseListener* MouseListener::SetOnExitListener(std::function<void()> callback)
{
	onExit_ = callback;
	return this;
}

void MouseListener::Reset()
{

}

void MouseListener::OnUpdate()
{
	Collider* collider = GetOwner()->GetComponent<Collider>();
	if (collider&&collider->GetID() == typeid(AABBCollider))
	{
		
		const Point2F& currentMousePos = RG2Input->GetMousePos();
		const bool last = ((AABBCollider*)collider)->GetRectWithWorldPos().IsIntersect(lastMousePos_);
		const bool current = ((AABBCollider*)collider)->GetRectWithWorldPos().IsIntersect(currentMousePos);
		
		if (last && current)
		{
			onOver_();
			if (RG2Input->GetMouseState(MouseCode::MOUSE_LBUTTON) == KEYSTATE_ENTER)
				onClick_();
		}
			
		else if (!last && current)
			onEnter_();
		else if (last && !current)
			onExit_();


	}
	lastMousePos_ = RG2Input->GetMousePos();
}


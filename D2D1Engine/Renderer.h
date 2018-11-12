#pragma once
#include"Component.h"
class Renderer : public Component
{
public:
	Renderer(Entity* owner) : Component(owner) {}
	~Renderer() override {}

public:
	virtual void Reset(void) override {}
	virtual std::type_index GetID(void) override = 0 { return typeid(Renderer); }
	static std::type_index GetFamilyID(void) { return typeid(Renderer); }

public:

	virtual void OnUpdate(void) override {}
	virtual void OnRender(void) override {}

};


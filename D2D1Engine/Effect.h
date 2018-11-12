#pragma once
#include "Component.h"
#include"EffectInfo.h"
class Effect :
	public Component
{
private:
	//�� ���Ϳ� �� ������� ����Ʈ�� �����
	std::vector<EffectInfo* > effects_;
public:
	Effect(Entity* owner);
	virtual ~Effect() override;

public:
	virtual void Reset(void) override {}
	virtual std::type_index GetID(void) override { return typeid(Effect); }
	static std::type_index GetFamilyID(void) { return typeid(Effect); }

	Effect* PushEffectInfo(EffectInfo* effectInfo);
	Effect* PopEffectInfo();

	std::vector<EffectInfo*>& GetEffects() { return effects_; }

	ID2D1Image* GetOutputImage(ID2D1Image* input);

	virtual void OnUpdate(void) override {}
};


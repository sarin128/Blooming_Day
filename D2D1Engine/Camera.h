#pragma once
#include"Math.h"
#include<list>
class Entity;
class Camera 
{
private:
	Entity* scene_;
	Vec2F pos_;
public:
	Entity* target;
	Vec2F* Minimap;
	vector<Vec2F*> ButtonUI;
	Rect range;
	Vec2F offset;
	bool isSmoothFollowEnabled;
	//0ÀÌ»ף
	float smoothFollow;
	std::pair<float, float> shakeDuration;
	Vec2F shakeScale;
	float zoom;

public:
	Camera(Entity* scene);
	~Camera();
	Camera& Follow(Entity* target);
	Camera& UnFollow();
	Camera& SetOffset(const Vec2F& offset);
	Camera& SetRange(const Rect& range);
	Camera& SetPos(const Vec2F& Pos);
	Camera& SetMinimap(Vec2F& map);
	Camera& SetSmoothFollow(float smoothFollow);
	Camera& SetSmoothFollowEnabled(bool flag);
	Camera& SetZoom(float zoom);
	Camera& SetUI(Vec2F& UI);
	Camera& ClearUI();
	Camera& SetFaithScore(Vec2F& Score);
	Camera& SetHpViewer(Vec2F& TargetHp);
	Camera& SetUpgradeViewer(Vec2F& Upgrade);
	Vec2F* mapPos = NULL;
	Vec2F* FaithPos = NULL;
	Vec2F* HpViewerPos = NULL;
	Vec2F* UpgradeViewerPos = NULL;
	Camera& Shake(float duration, const Vec2F& scale);
	Vec2F GetOffset() const { return offset; }
	Rect GetRange() const { return range; }
	float GetSmoothFollow() const { return smoothFollow; }
	bool IsSmoothFollowEnabled() const { return isSmoothFollowEnabled; }
	float GetZoom() const { return zoom; }

	void Update();
};


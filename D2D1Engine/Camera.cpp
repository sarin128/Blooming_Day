#include "pch.h"
#include "Camera.h"
#include"App.h"
#include"Scene.h"
Camera::Camera(Entity* scene)
	:target(nullptr),
	range(-(int)RG2Window->GetSize().width, -(int)RG2Window->GetSize().height, RG2Window->GetSize().width, RG2Window->GetSize().height),
	offset(RG2Window->GetSize().width / 2, RG2Window->GetSize().height / 2),
	isSmoothFollowEnabled(true),
	smoothFollow(.15f),
	zoom(1.f),
	shakeDuration(0.f, 0.f),
	shakeScale(0.f, 0.f),
	scene_(scene),
	pos_(0, 0)
{

}


Camera::~Camera()
{
}

Camera& Camera::Follow(Entity* target)
{
	this->target = target;
	return *this;
}

Camera& Camera::UnFollow()
{
	target = nullptr;
	return *this;
}
Camera& Camera::SetOffset(const Vec2F & offset)
{
	this->offset = offset;
	return *this;
}
Camera& Camera::SetRange(const Rect & range)
{
	this->range = range;
	return *this;
}
Camera& Camera::SetSmoothFollow(float smoothFollow)
{
	this->smoothFollow = smoothFollow;
	return *this;
}
Camera& Camera::SetSmoothFollowEnabled(bool flag)
{
	this->isSmoothFollowEnabled = flag;
	return *this;
}

Camera& Camera::SetZoom(float zoom)
{
	this->zoom = zoom;

	return *this;
}


Camera& Camera::Shake(float duration, const Vec2F& scale)
{
	shakeDuration.first =0.f;
	shakeDuration.second = duration;
	shakeScale = scale;
	return *this;
}

Camera& Camera::SetPos(const Vec2F& Pos) {
	pos_.x += Pos.x;
	pos_.y += Pos.y;
	scene_->pos = (pos_);
	return *this;
}

Camera& Camera::SetMinimap(Vec2F& map) {
	mapPos = &map;
	return* this;
}
Camera& Camera::SetUI(Vec2F& UI) {
	ButtonUI.push_back(&UI);
	return* this;
}

Camera& Camera::ClearUI() {
	ButtonUI.clear();
	return*this;
}
Camera& Camera::SetFaithScore(Vec2F& Score) {
	FaithPos = &Score;
	return*this;
}

Camera& Camera::SetHpViewer(Vec2F& TargetHp) {
	HpViewerPos = &TargetHp;
	return*this;
}

Camera& Camera::SetUpgradeViewer(Vec2F& Upgrade) {
	UpgradeViewerPos = &Upgrade;
	return* this;
}

void Camera::Update()
{
	Rect area = Rect(0, 0, RG2Settings->windowWidth / zoom, RG2Settings->windowHeight / zoom).Offset(offset - offset / zoom);
	scene_->scale = Vec2F(zoom, zoom);
	scene_->scaleCenter = offset;
	if (FaithPos != NULL) {
		FaithPos->x = -((pos_.x - RG2Window->GetWin().right) + 600);
		FaithPos->y = -((pos_.y - RG2Window->GetWin().bottom) + 300);
	}
	if (HpViewerPos != NULL) {
		HpViewerPos->x = -((pos_.x - RG2Window->GetWin().right) + 1500);
		HpViewerPos->y = -((pos_.y - RG2Window->GetWin().bottom) + 300);
	}
	if (mapPos != NULL) {
		mapPos->x = -((pos_.x - RG2Window->GetWin().right)+400);
		mapPos->y = -((pos_.y - RG2Window->GetWin().bottom)+420);
	}
	if (UpgradeViewerPos != NULL) {
		UpgradeViewerPos->x = -((pos_.x - RG2Window->GetWin().right) + 1500);
		UpgradeViewerPos->y = -((pos_.y - RG2Window->GetWin().bottom) + 350);
	}
	if (ButtonUI.size() != 0) {
		for (int i = 0,j=0; i < ButtonUI.size(); i++,j+=135) {
			ButtonUI.at(i)->x = -((pos_.x - RG2Window->GetWin().right) + (1300-j));
			ButtonUI.at(i)->y = -((pos_.y - RG2Window->GetWin().bottom) + 300);
		}
	}
	if (target)
	{

		if (isSmoothFollowEnabled)
			pos_ += (-(target->pos) + offset - pos_) * smoothFollow;
		else
			pos_ = (-target->pos + offset);

		pos_.x = Clamp(pos_.x, -range.right + area.right, -range.left + area.left);
		pos_.y = Clamp(pos_.y, -range.bottom + area.bottom, -range.top + area.top);
		scene_->pos = (pos_ );
	}

	
	if (shakeDuration.first < shakeDuration.second)
	{
		scene_->pos.x += Random(0.f, shakeScale.x) - shakeScale.x / 2;
		scene_->pos.y += Random(0.f, shakeScale.y) - shakeScale.y / 2;
		shakeDuration.first += DT;

	}

		
}

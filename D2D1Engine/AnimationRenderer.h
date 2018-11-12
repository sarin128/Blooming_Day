#pragma once
#include "Renderer.h"
#include"Texture.h"
class AnimationRenderer :
	public Renderer
{
private:
	std::vector<Texture*> textures_;
public:
	Rect visibleRect;
	float fps;
	float currentFrame;
	bool isLoop;
	bool isPaused;
public:
	AnimationRenderer(Entity *owner,
		float fps = 25.f,
		bool isLoop = true,
		bool isReversed = false);
	~AnimationRenderer() override;

	Texture* GetTexture(void) const;
	const std::vector<Texture*>& GetTextures(void) const;
	int GetTexturesCount(void)const;
	
	AnimationRenderer* PushTexture(const std::string& path, bool resetVisibleRect = true);
	AnimationRenderer* PushTexture(Texture* texture, bool resetVisibleRect = true);
	//지정한 폴더 하위에 있는 [0~frame].png를 불러온다.
	AnimationRenderer* PushTextures(const std::string& folderPath, int frame , bool resetVisibleRect = true);
	AnimationRenderer* ResetVisibleRect();
	AnimationRenderer* SetVisibleRect(const Rect& rect) { visibleRect = rect; return this; }
	AnimationRenderer* SetTexture(int index, const std::string& path);
	AnimationRenderer* SetTexture(int index, Texture* texture);

	AnimationRenderer* SetFPS(float fps) { this->fps = fps; return this; }
	AnimationRenderer* SetCurrentFrame(float cf) { this->currentFrame = currentFrame; return this; }
	AnimationRenderer* SetLoop(bool flag) { isLoop = flag; return this; }
	AnimationRenderer* Pause() { isPaused = true; return this; }
	AnimationRenderer* Resume() { isPaused = false; return this; }

	Texture* GetTexture(int index) const;
	Texture* GetCurrentTexture() const;
	Rect GetVisibleRect() const { return visibleRect; }
	float GetFPS() const {	return fps;}
	float GetCurrentFrame() const { return currentFrame; }
	bool IsLoop() const { return isLoop; }
	bool IsPaused() const { return isPaused; }


	void Reset(void) override;
	std::type_index GetID(void) override { return typeid(AnimationRenderer); }
	static std::type_index GetFamilyID(void) { return typeid(Renderer); }

	void OnUpdate(void) override;
	void OnRender(void) override;

};


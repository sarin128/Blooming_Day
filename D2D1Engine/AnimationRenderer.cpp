#include "pch.h"
#include "AnimationRenderer.h"
#include"App.h"
using namespace std::experimental::filesystem;
AnimationRenderer::AnimationRenderer(Entity * owner, float fps, bool isLoop, bool isReversed)
	:Renderer(owner),
	fps(fps),
	isLoop(isLoop),
	visibleRect(0.f,0.f,0.f,0.f),
	currentFrame(0.f),
	isPaused(false)
{
}

AnimationRenderer::~AnimationRenderer()
{
}
Texture* AnimationRenderer::GetTexture(void) const
{
	return textures_[(int)currentFrame];
}

const std::vector<Texture*>& AnimationRenderer::GetTextures(void) const
{
	return textures_;
}

int AnimationRenderer::GetTexturesCount(void) const
{
	return textures_.size();
}

AnimationRenderer* AnimationRenderer::PushTexture(const std::string & path, bool resetVisibleRect)
{
	textures_.push_back( RG2TextureManager->Load(path));
	if (resetVisibleRect)
		ResetVisibleRect();
	return this;
}

AnimationRenderer* AnimationRenderer::PushTexture(Texture * texture, bool resetVisibleRect)
{
	textures_.push_back(texture);
	if (resetVisibleRect)
		ResetVisibleRect();
	return this;
}

AnimationRenderer* AnimationRenderer::PushTextures(const std::string & folderPath, int frame, bool resetVisibleRect )
{
	std::string extension;
	std::string tmp = folderPath + "/0" ;

	if (exists(tmp + ".png"))
		extension = ".png";
	else if (exists(tmp + ".dds"))
		extension = ".dds";
	else if (exists(tmp + ".jpg"))
		extension = ".jpg";
	else if (exists(tmp + ".jpeg"))
		extension = ".jpeg";
	else if (exists(tmp + ".gif"))
		extension = ".gif";
	else if (exists(tmp + ".bmp"))
		extension = ".bmp";

	for (int i = 0; i < frame; i++)
	{
		Texture* t = RG2TextureManager->Load(folderPath + "/" + std::to_string(i)+extension);
		textures_.push_back(t);
	}
	if (resetVisibleRect)
		ResetVisibleRect();
	return this;
	
}
AnimationRenderer* AnimationRenderer::ResetVisibleRect()
{
	visibleRect = Rect(0.f, 0.f, textures_[0]->GetSize().width, textures_[0]->GetSize().height);
	return this;
}

AnimationRenderer* AnimationRenderer::SetTexture(int index, const std::string & path)
{
	textures_[index] = RG2TextureManager->Load(path);
	return this;
}

AnimationRenderer* AnimationRenderer::SetTexture(int index, Texture * texture)
{
	textures_[index] = texture;
	return this;
}

Texture * AnimationRenderer::GetTexture(int index) const
{
	return textures_[index];
}

Texture * AnimationRenderer::GetCurrentTexture() const
{
	return textures_[currentFrame];
}

void AnimationRenderer::Reset(void)
{
	textures_.clear();
	visibleRect = Rect();
}

void AnimationRenderer::OnUpdate(void)
{
	if (isPaused) return;
	currentFrame += fps * DT;

	if (currentFrame >= textures_.size())
	{
		currentFrame = 0.0f;
		if (!isLoop) isPaused = true;
	}
}

void AnimationRenderer::OnRender(void)
{
	if (!textures_.empty())
	{
		textures_[currentFrame]->Render(visibleRect);
	}
}

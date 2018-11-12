#include "pch.h"
#include "SpriteRenderer.h"
#include"App.h"
#include"TextureManager.h"
#include"Effect.h"
SpriteRenderer::SpriteRenderer(Entity* owner) : Renderer(owner), texture_(nullptr),
visibleRect(0.f, 0.f, 0.f, 0.f)
{
}
SpriteRenderer::~SpriteRenderer() {}
SpriteRenderer* SpriteRenderer::SetTexture(const std::string& path,  bool resetVisibleRect)
{
	texture_ = App::GetInstance()->GetTextureManager()->Load(path);
	if (resetVisibleRect)
		ResetVisibleRect();
	return this;
}
SpriteRenderer* SpriteRenderer::SetTexture(Texture * texture, bool resetVisibleRect)
{
	texture_ = texture;
	if (resetVisibleRect)
		ResetVisibleRect();
	return this;
}
SpriteRenderer* SpriteRenderer::ResetVisibleRect()
{
	visibleRect = Rect(0.f, 0.f, texture_->GetSize().width, texture_->GetSize().height);
	return this;
}
void SpriteRenderer::Reset(void)
{
	texture_ = nullptr;
	visibleRect = Rect();
}
void SpriteRenderer::OnRender(void)
{
	if (texture_)
	{
		Effect* e = GetOwner()->GetComponent<Effect>();
		if(!e)
			texture_->Render(visibleRect);
		else
		{	
			RG2Graphics->GetDeviceContext()
				->DrawImage(e->GetOutputImage(texture_->GetBitmap()),
					nullptr,
					&visibleRect,
					D2D1_INTERPOLATION_MODE_LINEAR,
					D2D1_COMPOSITE_MODE_SOURCE_OVER);
		}
	}
}
#pragma once
#include "Renderer.h"
#include"Texture.h"
#include"Math.h"
class SpriteRenderer :
	public Renderer
{
private:
	Texture* texture_;
public:
	Rect visibleRect;
public:
	SpriteRenderer(Entity *owner);
	~SpriteRenderer() override;

	Texture* GetTexture(void) const
	{
		return texture_;
	}
	SpriteRenderer* SetTexture(const std::string& path, bool resetVisibleRect = true);
	SpriteRenderer* SetTexture(Texture* texture, bool resetVisibleRect = true);
	SpriteRenderer* ResetVisibleRect();
	SpriteRenderer* SetVisibleRect(const Rect& rect) { visibleRect = rect; return this; }
	Rect GetVisibleRect() const { return visibleRect; }
	void Reset(void) override;
	std::type_index GetID(void) override { return typeid(SpriteRenderer); }
	static std::type_index GetFamilyID(void) { return typeid(Renderer); }

	void OnRender(void) override;

};


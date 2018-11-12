#pragma once
#include "Renderer.h"
#include"Math.h"
class TextRenderer :
	public Renderer, IDWriteTextRenderer
{
private:
	IDWriteTextLayout* layout_;
	IDWriteTextFormat* format_;
	std::string text_;
public:
	Color outlineColor;
	Color fillColor;
	//Å×µÎ¸®µÎ²²
	float outlineWidth;
public:
	TextRenderer(Entity *owner, const std::string& fontName = "¸¼Àº °íµñ", int size = 12, 
		float outlineWidth = 0.f,
		const Color& fill = Color(1.f,1.f,1.f,1.f),
		const Color& outline = Color(0.f,0.f,0.f,0.f), const std::string& text = "");
	~TextRenderer() override;


	std::type_index GetID(void) override { return typeid(TextRenderer); }
	static std::type_index GetFamilyID(void) { return typeid(Renderer); }


	TextRenderer* SetText(const std::string& text);
	TextRenderer* SetOutlineColor(const Color& color) { outlineColor = color; return this; }
	TextRenderer* SetFillColor(const Color& color) { fillColor = color; return this; }
	TextRenderer* SetOutlineWidth(float width) { outlineWidth = width; return this; }
	std::string GetText() const { return text_; }
	Color GetOutlineColor() const { return outlineColor; }
	Color GetFillColor() const { return outlineColor; }
	float GetOutlineWidth() const { return outlineWidth; }



	void OnRender(void) override;

#pragma region MethodsDerivedFromIDWriteTextRenderer
	IFACEMETHOD(DrawGlyphRun)(
		_In_opt_ void* ,FLOAT ,FLOAT ,DWRITE_MEASURING_MODE ,
		_In_ DWRITE_GLYPH_RUN const* ,_In_ DWRITE_GLYPH_RUN_DESCRIPTION const* ,
		IUnknown* 
		);
	IFACEMETHOD(DrawUnderline)(
		_In_opt_ void* clientDrawingContext,
		FLOAT baselineOriginX,
		FLOAT baselineOriginY,
		_In_ DWRITE_UNDERLINE const* underline,
		IUnknown* clientDrawingEffect
		);

	IFACEMETHOD(DrawStrikethrough)(
		_In_opt_ void* clientDrawingContext,
		FLOAT baselineOriginX,
		FLOAT baselineOriginY,
		_In_ DWRITE_STRIKETHROUGH const* strikethrough,
		IUnknown* clientDrawingEffect
		);

	IFACEMETHOD(DrawInlineObject)(
		_In_opt_ void* clientDrawingContext,
		FLOAT originX,
		FLOAT originY,
		IDWriteInlineObject* inlineObject,
		BOOL isSideways,
		BOOL isRightToLeft,
		IUnknown* clientDrawingEffect
		);

	IFACEMETHOD_(unsigned long, AddRef) ();
	IFACEMETHOD_(unsigned long, Release) ();
	IFACEMETHOD(QueryInterface) (
		IID const& riid,
		void** ppvObject
		);
	IFACEMETHOD(IsPixelSnappingDisabled)(
		_In_opt_ void* clientDrawingContext,
		_Out_ BOOL* isDisabled
		);
	IFACEMETHOD(GetCurrentTransform)(
		_In_opt_ void* clientDrawingContext,
		_Out_ DWRITE_MATRIX* transform
		);

	IFACEMETHOD(GetPixelsPerDip)(
		_In_opt_ void* clientDrawingContext,
		_Out_ FLOAT* pixelsPerDip
		);
#pragma endregion
	
};


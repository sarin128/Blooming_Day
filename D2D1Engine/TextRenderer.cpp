#include "pch.h"
#include "TextRenderer.h"
#include"App.h"

TextRenderer::TextRenderer(Entity* owner, const std::string& fontName , int size,
	float outlineWidth,
	const Color& fill ,
	const Color& outline, const std::string& text)
	:Renderer(owner),
	outlineColor(outline),
	fillColor(fill),
	outlineWidth(outlineWidth)
	
{
	RG2Graphics->dwFactory_->CreateTextFormat(
		Widen(fontName).c_str(),
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		size,
		L"ko",
		&format_
	);
	RG2Graphics->dwFactory_->CreateTextLayout(
		Widen(text).c_str(),
		Widen(text).length(), 
		format_,
		RG2Graphics->GetDeviceContext()->GetSize().width,
		RG2Graphics->GetDeviceContext()->GetSize().height,
		&layout_);
}


TextRenderer::~TextRenderer()
{
	format_->Release();
	layout_->Release();
}



TextRenderer* TextRenderer::SetText(const std::string& text)
{
	//같으면 아무것도 안한다.
	if (text_ == text)
		return this;
	text_ = text;
	auto t = Widen(text).c_str();
	layout_->Release();
	RG2Graphics->dwFactory_->CreateTextLayout(Widen(text).c_str(), Widen(text).length(), format_, RG2Window->GetSize().width,
		RG2Window->GetSize().height, &layout_);
	return this;
}

void TextRenderer::OnRender(void)
{
	/*RG2Graphics->GetDeviceContext()->DrawTextLayout(
		D2D1::Point2F(0, 0),
		layout_,
	)*/

	RG2Graphics->fillBrush_->SetColor(fillColor );
	RG2Graphics->outlineBrush_->SetColor(outlineColor );
	layout_->Draw(RG2Graphics->GetDeviceContext(),
		this,
		0.f,
		0.f);
	
}


HRESULT TextRenderer::DrawGlyphRun(void *cdc, FLOAT box, FLOAT boy, 
	DWRITE_MEASURING_MODE mm, const DWRITE_GLYPH_RUN * gr, 
	const DWRITE_GLYPH_RUN_DESCRIPTION * grd, IUnknown * cde)
{
	ID2D1PathGeometry* pathGeometry = nullptr;
	HR(
		RG2Graphics->factory_->CreatePathGeometry(
			&pathGeometry
		)
	);
	// Write to the path geometry using the geometry sink.
	ID2D1GeometrySink* sink = nullptr;

	HR(
		pathGeometry->Open(
			&sink
		)
	);
	HR(
		gr->fontFace->GetGlyphRunOutline(
			gr->fontEmSize,
			gr->glyphIndices,
			gr->glyphAdvances,
			gr->glyphOffsets,
			gr->glyphCount,
			gr->isSideways,
			gr->bidiLevel % 2,
			sink
		)
	);

	HR(
		sink->Close()
	);
	D2D1::Matrix3x2F const matrix = D2D1::Matrix3x2F(
		1.0f, 0.0f,
		0.0f, 1.0f,
		box,boy
	);
	ID2D1TransformedGeometry* transformedGeometry = nullptr;
	HR(
		RG2Graphics->factory_->CreateTransformedGeometry(
			pathGeometry,
			&matrix,
			&transformedGeometry
		)
	);

	// Draw the outline of the glyph run

	RG2Graphics->deviceContext_->DrawGeometry(
		transformedGeometry,
		RG2Graphics->outlineBrush_,
		outlineWidth
	);

	// Fill in the glyph run
	RG2Graphics->deviceContext_->FillGeometry(
		transformedGeometry,
		RG2Graphics->fillBrush_
	);
	pathGeometry->Release();
	sink->Release();
	transformedGeometry->Release();

	return S_OK;
}

HRESULT TextRenderer::DrawUnderline(void* cdc, FLOAT box, FLOAT boy, const DWRITE_UNDERLINE* underline, IUnknown* cde)
{
	HRESULT hr;

	D2D1_RECT_F rect = D2D1::RectF(
		0,
		underline->offset,
		underline->width,
		underline->offset + underline->thickness
	);
	//------------------------------------------
	ID2D1RectangleGeometry* rectangleGeometry;
	hr = RG2Graphics->factory_->CreateRectangleGeometry(
		&rect,
		&rectangleGeometry
	);

	// Initialize a matrix to translate the origin of the underline
	D2D1::Matrix3x2F const matrix = D2D1::Matrix3x2F(
		1.0f, 0.0f,
		0.0f, 1.0f,
		box,boy
	);

	ID2D1TransformedGeometry* transformedGeometry;
	if (SUCCEEDED(hr))
	{
		hr = RG2Graphics->factory_->CreateTransformedGeometry(
			rectangleGeometry,
			&matrix,
			&transformedGeometry
		);
	}

	// Draw the outline of the rectangle
	RG2Graphics->deviceContext_->DrawGeometry(
		transformedGeometry,
		RG2Graphics->outlineBrush_
	);

	// Fill in the rectangle
	RG2Graphics->deviceContext_->FillGeometry(
		transformedGeometry,
		RG2Graphics->fillBrush_
	);
	rectangleGeometry->Release();
	transformedGeometry->Release();
	return S_OK;
}
HRESULT TextRenderer::DrawStrikethrough(
	_In_opt_ void* cdc,
	FLOAT box,
	FLOAT boy,
	_In_ DWRITE_STRIKETHROUGH const* strikethrough,
	IUnknown* cde
)
{
	HRESULT hr;

	D2D1_RECT_F rect = D2D1::RectF(
		0,
		strikethrough->offset,
		strikethrough->width,
		strikethrough->offset + strikethrough->thickness
	);

	ID2D1RectangleGeometry* rectangleGeometry;
	hr = RG2Graphics->factory_->CreateRectangleGeometry(
		&rect,
		&rectangleGeometry
	);

	// Initialize a matrix to translate the origin of the strikethrough
	D2D1::Matrix3x2F const matrix = D2D1::Matrix3x2F(
		1.0f, 0.0f,
		0.0f, 1.0f,
		box, boy
	);

	ID2D1TransformedGeometry* transformedGeometry;
	if (SUCCEEDED(hr))
	{
		hr = RG2Graphics->factory_->CreateTransformedGeometry(
			rectangleGeometry,
			&matrix,
			&transformedGeometry
		);
	}

	// Draw the outline of the rectangle

	RG2Graphics->deviceContext_->DrawGeometry(
		transformedGeometry,
		RG2Graphics->outlineBrush_
	);

	// Fill in the rectangle
	RG2Graphics->deviceContext_->FillGeometry(
		transformedGeometry,
		RG2Graphics->fillBrush_
	);
	rectangleGeometry->Release();
	transformedGeometry->Release();
	return S_OK;
}
HRESULT TextRenderer::DrawInlineObject(
	_In_opt_ void* ,
	FLOAT ,
	FLOAT ,
	IDWriteInlineObject* i,
	BOOL ,
	BOOL ,
	IUnknown*
)
{
	// Not implemented
	return E_NOTIMPL;
}



IFACEMETHODIMP TextRenderer::QueryInterface(
	IID const& riid,
	void** ppvObject
)
{
	if (__uuidof(IDWriteTextRenderer) == riid)
	{
		*ppvObject = this;
	}
	else if (__uuidof(IDWritePixelSnapping) == riid)
	{
		*ppvObject = this;
	}
	else if (__uuidof(IUnknown) == riid)
	{
		*ppvObject = this;
	}
	else
	{
		*ppvObject = nullptr;
		return E_FAIL;
	}

	this->AddRef();

	return S_OK;
}


ULONG TextRenderer::AddRef(void)
{
	return 0;
}

ULONG TextRenderer::Release(void)
{
	return 0;
}

HRESULT TextRenderer::IsPixelSnappingDisabled(void *clientDrawingContext, BOOL *isDisabled)
{
	*isDisabled = FALSE;
	return S_OK;
}
IFACEMETHODIMP TextRenderer::GetCurrentTransform(
	_In_opt_ void* clientDrawingContext,
	_Out_ DWRITE_MATRIX* transform
)
{
	// forward the render target's transform
	RG2Graphics->GetDeviceContext()->GetTransform(reinterpret_cast<D2D1_MATRIX_3X2_F*>(transform));
	return S_OK;
}

////  CustomTextRenderer::GetPixelsPerDip
////
////  This returns the number of pixels per DIP.
////

IFACEMETHODIMP TextRenderer::GetPixelsPerDip(
	_In_opt_ void* clientDrawingContext,
	_Out_ FLOAT* pixelsPerDip
)
{
	float x, yUnused;

	RG2Graphics->GetDeviceContext()->GetDpi(&x, &yUnused);
	*pixelsPerDip = x / 96;

	return S_OK;
}

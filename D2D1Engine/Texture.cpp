#include "pch.h"
#include "Texture.h"
#include"App.h"

Texture::Texture(const std::string& path)
	:bitmap_(nullptr)
{
	IWICBitmapDecoder* decoder = nullptr;
	HR(
		App::GetInstance()->GetTextureManager()->wicFactory_->CreateDecoderFromFilename(
			Widen(path).c_str(),
			nullptr,
			GENERIC_READ,
			WICDecodeMetadataCacheOnDemand,
			&decoder
		)
	);
	IWICBitmapFrameDecode* frame = nullptr;
	HR(
		decoder->GetFrame(0, &frame)
	);
	
	IWICFormatConverter* convertedSrcBmp = nullptr;
	HR(
		App::GetInstance()->GetTextureManager()->wicFactory_->CreateFormatConverter(
			&convertedSrcBmp
		)
	);

	HR(
		convertedSrcBmp->Initialize(
			frame,
			GUID_WICPixelFormat32bppPRGBA,
			WICBitmapDitherTypeNone,
			nullptr,
			0.f,
			WICBitmapPaletteTypeCustom
		)
	);
	
	HR(
		App::GetInstance()->GetGraphics()->GetDeviceContext()->CreateBitmapFromWicBitmap(
			convertedSrcBmp,
			&bitmap_
		)
	);

	decoder->Release();
	convertedSrcBmp->Release();
	frame->Release();
	
}

Texture::~Texture()
{
	bitmap_->Release();
}

Size2F Texture::GetSize()
{
	return Size2F(bitmap_->GetSize().width, bitmap_->GetSize().height);
}

void Texture::Render(const Rect& visibleRect)
{
	App::GetInstance()->GetGraphics()->GetDeviceContext()->DrawBitmap(
		bitmap_,
		nullptr,
		1.f,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		&visibleRect
	);

}

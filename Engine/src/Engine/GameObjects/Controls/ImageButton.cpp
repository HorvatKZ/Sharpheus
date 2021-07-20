#include "pch.h"
#include "ImageButton.hpp"
#include "Engine/Renderer/Renderer.hpp"


namespace Sharpheus {

	SPH_START_CLASSINFO(ImageButton, "imagebutton.png")
		SPH_PROVIDE_IMAGE(ImageButton, "Image", GetImg, SetImg, SetImgFromPath)
		SPH_PROVIDE_COLOR(ImageButton, "Tint", GetColor, SetColor)
		SPH_PROVIDE_STRING(ImageButton, "Text", GetText, SetText)
		SPH_PROVIDE_FONT(ImageButton, "Font", GetFont, SetFont, SetFontByName, SetFontByPath)
		SPH_PROVIDE_COLOR(ImageButton, "Font color", GetFontColor, SetFontColor)
		SPH_PROVIDE_UFLOAT(ImageButton, "Font size", GetFontSize, SetFontSize)
		SPH_PROVIDE_FONTSTYLE(ImageButton, "Font style", GetFontStyle, SetFontStyle)
		SPH_PROVIDE_BOOL(ImageButton, "Use clicked", DoesUseClickedColors, SetUseClickedColors)
		SPH_PROVIDE_IMAGE(ImageButton, "Clicked image", GetClickedImg, SetClickedImg, SetClickedImgFromPath)
		SPH_PROVIDE_COLOR(ImageButton, "Clicked tint", GetClickedColor, SetClickedColor)
		SPH_PROVIDE_COLOR(ImageButton, "Clicked font color", GetClickedFontColor, SetClickedFontColor)
	SPH_END_CLASSINFO


	ImageButton::ImageButton(GameObject* parent, const std::string& name)
		: ButtonBase(parent, name)
	{
		color = Color::White;
		clickedColor = Color::White;
	}


	ImageButton::~ImageButton()
	{
	}


	void ImageButton::CopyFrom(GameObject* other)
	{
		SPH_CHECKTYPE(other, ImageButton);

		ButtonBase::CopyFrom(other);
		ImageButton* trueOther = (ImageButton*)other;
		SetImg(trueOther->img);
		SetClickedImg(trueOther->clickedImg);
	}


	void ImageButton::SetImgFromPath(const std::string& path, bool filtered)
	{
		SetImg(ResourceManager::GetImage(path, filtered));
	}


	void ImageButton::SetClickedImgFromPath(const std::string& path, bool filtered)
	{
		SetClickedImg(ResourceManager::GetImage(path, filtered));
	}


	bool ImageButton::Load(FileLoader& fl)
	{
		ButtonBase::Load(fl);
		Image* tempImg;
		fl.Read(&tempImg);
		SetImg(tempImg);
		fl.Read(&tempImg);
		SetClickedImg(tempImg);
		return fl.GetStatus();
	}


	bool ImageButton::Save(FileSaver& fs)
	{
		ButtonBase::Save(fs);
		fs.Write(img);
		fs.Write(clickedImg);
		return fs.GetStatus();
	}


	void ImageButton::Render()
	{
		bool useClickedNow = isCurrentlyClicked && useClickedColors;
		if (useClickedNow && clickedImg != nullptr) {
			clickedImg->Render(shape->GetCorners(), useClickedNow ? clickedColor : color);
		} else if (img != nullptr) {
			img->Render(shape->GetCorners(), useClickedNow ? clickedColor : color);
		}

		if (font != nullptr && !text.empty()) {
			font->Render(text, worldTrafo.pos, xAxis, yAxis, fontSize, useClickedNow ? clickedFontColor : fontColor, fontStyle);
		}
	}


	void ImageButton::UpdateSizer()
	{
		if (img != nullptr) {
			SetSizer(img->GetWidth(), img->GetHeight());
		} else {
			SetSizer(0.f, 0.f);
		}
	}

}
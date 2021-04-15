#include "pch.h"
#include "ImageButton.hpp"
#include "Engine/Renderer/Renderer.hpp"


namespace Sharpheus {

	ClassInfo ImageButton::classInfo("ImageButton", "imagebutton.png", {
		new ImageProvider<ImageButton>("Image", SPH_BIND_GETTER(ImageButton::GetImg), SPH_BIND_SETTER(ImageButton::SetImg), SPH_BIND_3(ImageButton::SetImgFromPath)),
		new ColorProvider<ImageButton>("Tint", SPH_BIND_GETTER(ImageButton::GetColor), SPH_BIND_SETTER(ImageButton::SetColor)),
		new StringProvider<ImageButton>("Text", SPH_BIND_GETTER(ImageButton::GetText), SPH_BIND_SETTER(ImageButton::SetText)),
		new FontProvider<ImageButton>("Font", SPH_BIND_GETTER(ImageButton::GetFont), SPH_BIND_SETTER(ImageButton::SetFont),
			SPH_BIND_SETTER(ImageButton::SetFontByName), SPH_BIND_3(ImageButton::SetFontByPath), ResourceManager::GetFontTable()),
		new ColorProvider<ImageButton>("Font color", SPH_BIND_GETTER(ImageButton::GetFontColor), SPH_BIND_SETTER(ImageButton::SetFontColor)),
		new UFloatProvider<ImageButton>("Font size", SPH_BIND_GETTER(ImageButton::GetFontSize), SPH_BIND_SETTER(ImageButton::SetFontSize)),
		new FontStyleProvider<ImageButton>("Font style", SPH_BIND_GETTER(ImageButton::GetFontStyle), SPH_BIND_SETTER(ImageButton::SetFontStyle)),
		new BoolProvider<ImageButton>("Use clicked", SPH_BIND_GETTER(ImageButton::DoesUseClickedColors), SPH_BIND_SETTER(ImageButton::SetUseClickedColors)),
		new ImageProvider<ImageButton>("Clicked image", SPH_BIND_GETTER(ImageButton::GetClickedImg), SPH_BIND_SETTER(ImageButton::SetClickedImg), SPH_BIND_3(ImageButton::SetClickedImgFromPath)),
		new ColorProvider<ImageButton>("Clicked tint", SPH_BIND_GETTER(ImageButton::GetClickedColor), SPH_BIND_SETTER(ImageButton::SetClickedColor)),
		new ColorProvider<ImageButton>("Clicked font color", SPH_BIND_GETTER(ImageButton::GetClickedFontColor), SPH_BIND_SETTER(ImageButton::SetClickedFontColor))
	});


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
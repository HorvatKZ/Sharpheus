#include "pch.h"
#include "ButtonBase.hpp"
#include "Engine/Renderer/Renderer.hpp"


namespace Sharpheus {

	ButtonBase::ButtonBase(GameObject* parent, const std::string& name)
		: Control(parent, name, new Rect())
	{
	}


	ButtonBase::~ButtonBase()
	{
	}


	void ButtonBase::CopyFrom(GameObject* other)
	{
		if (!other->Is(Type::Button) && !other->Is(Type::ImageButton)) {
			SPH_ERROR("\"{0}\" tries to copy from different type \"{1}\"", name, other->GetName());
			return;
		}

		Control::CopyFrom(other);
		ButtonBase* trueOther = (ButtonBase*)other;
		color = trueOther->color;
		clickedColor = trueOther->clickedColor;
		clickedFontColor = trueOther->clickedFontColor;
		useClickedColors = trueOther->useClickedColors;
	}


	bool ButtonBase::Load(FileLoader& fl)
	{
		Control::Load(fl);
		fl.Read(color);
		fl.Read(clickedColor);
		fl.Read(clickedFontColor);
		fl.Read(useClickedColors);
		return fl.GetStatus();
	}


	bool ButtonBase::Save(FileSaver& fs)
	{
		Control::Save(fs);
		fs.Write(color);
		fs.Write(clickedColor);
		fs.Write(clickedFontColor);
		fs.Write(useClickedColors);
		return fs.GetStatus();
	}


	void ButtonBase::ChangeOnClick()
	{
		++clickNum;
	}


	void ButtonBase::ChangeOnRelease()
	{
	}

}
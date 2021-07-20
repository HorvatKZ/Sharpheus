#include "pch.h"
#include "CheckBox.hpp"


namespace Sharpheus {

	SPH_START_CLASSINFO(CheckBox, "checkbox.png")
		SPH_PROVIDE_IMAGE(CheckBox, "Checked image", GetCheckedImg, SetCheckedImg, SetCheckedImgFromPath)
		SPH_PROVIDE_IMAGE(CheckBox, "Unchecked image", GetUncheckedImg, SetUncheckedImg, SetUncheckedImgFromPath)
		SPH_PROVIDE_COLOR(CheckBox, "Tint", GetTint, SetTint)
		SPH_PROVIDE_STRING(CheckBox, "Text", GetText, SetText)
		SPH_PROVIDE_BOOL(CheckBox, "On right", IsTextOnRight, SetTextOnRight)
		SPH_PROVIDE_FONT(CheckBox, "Font", GetFont, SetFont, SetFontByName, SetFontByPath)
		SPH_PROVIDE_COLOR(CheckBox, "Font color", GetFontColor, SetFontColor)
		SPH_PROVIDE_UFLOAT(CheckBox, "Font size", GetFontSize, SetFontSize)
		SPH_PROVIDE_FONTSTYLE(CheckBox, "Font style", GetFontStyle, SetFontStyle)
	SPH_END_CLASSINFO


	CheckBox::CheckBox(GameObject* parent, const std::string& name)
		: CheckBase(parent, name, new Rect())
	{
		SetCheckedImg(ResourceManager::GetImage("UI\\checkedbox.png", true));
		SetUncheckedImg(ResourceManager::GetImage("UI\\uncheckedbox.png", true));
	}


	CheckBox::~CheckBox()
	{
	}


	void CheckBox::CopyFrom(GameObject* other)
	{
		SPH_CHECKTYPE(other, CheckBox);

		CheckBase::CopyFrom(other);
	}


	void CheckBox::ChangeOnClick()
	{
		isChecked = !isChecked;
	}

}
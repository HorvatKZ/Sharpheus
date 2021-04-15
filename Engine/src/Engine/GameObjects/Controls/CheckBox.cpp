#include "pch.h"
#include "CheckBox.hpp"


namespace Sharpheus {

	ClassInfo CheckBox::classInfo("CheckBox", "checkbox.png", {
		new ImageProvider<CheckBox>("Checked image", SPH_BIND_GETTER(CheckBox::GetCheckedImg), SPH_BIND_SETTER(CheckBox::SetCheckedImg), SPH_BIND_3(CheckBox::SetCheckedImgFromPath)),
		new ImageProvider<CheckBox>("Unchecked image", SPH_BIND_GETTER(CheckBox::GetUncheckedImg), SPH_BIND_SETTER(CheckBox::SetUncheckedImg), SPH_BIND_3(CheckBox::SetUncheckedImgFromPath)),
		new StringProvider<CheckBox>("Text", SPH_BIND_GETTER(CheckBox::GetText), SPH_BIND_SETTER(CheckBox::SetText)),
		new BoolProvider<CheckBox>("On right", SPH_BIND_GETTER(CheckBox::IsTextOnRight), SPH_BIND_SETTER(CheckBox::SetTextOnRight)),
		new FontProvider<CheckBox>("Font", SPH_BIND_GETTER(CheckBox::GetFont), SPH_BIND_SETTER(CheckBox::SetFont),
			SPH_BIND_SETTER(CheckBox::SetFontByName), SPH_BIND_3(CheckBox::SetFontByPath), ResourceManager::GetFontTable()),
		new ColorProvider<CheckBox>("Font color", SPH_BIND_GETTER(CheckBox::GetFontColor), SPH_BIND_SETTER(CheckBox::SetFontColor)),
		new UFloatProvider<CheckBox>("Font size", SPH_BIND_GETTER(CheckBox::GetFontSize), SPH_BIND_SETTER(CheckBox::SetFontSize)),
		new FontStyleProvider<CheckBox>("Font style", SPH_BIND_GETTER(CheckBox::GetFontStyle), SPH_BIND_SETTER(CheckBox::SetFontStyle))
	});


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
#include "pch.h"
#include "RadioButton.hpp"


namespace Sharpheus {

	SPH_START_CLASSINFO(RadioButton, "radio.png")
		SPH_PROVIDE_UINT(RadioButton, "Group ID", GetGroupId, SetGroupId)
		SPH_PROVIDE_IMAGE(RadioButton, "Checked image", GetCheckedImg, SetCheckedImg, SetCheckedImgFromPath)
		SPH_PROVIDE_IMAGE(RadioButton, "Unchecked image", GetUncheckedImg, SetUncheckedImg, SetUncheckedImgFromPath)
		SPH_PROVIDE_COLOR(RadioButton, "Tint", GetTint, SetTint)
		SPH_PROVIDE_STRING(RadioButton, "Value", GetText, SetText)
		SPH_PROVIDE_BOOL(RadioButton, "On right", IsTextOnRight, SetTextOnRight)
		SPH_PROVIDE_FONT(RadioButton, "Font", GetFont, SetFont, SetFontByName, SetFontByPath)
		SPH_PROVIDE_COLOR(RadioButton, "Font color", GetFontColor, SetFontColor)
		SPH_PROVIDE_UFLOAT(RadioButton, "Font size", GetFontSize, SetFontSize)
		SPH_PROVIDE_FONTSTYLE(RadioButton, "Font style", GetFontStyle, SetFontStyle)
	SPH_END_CLASSINFO

	std::unordered_map<ID, std::vector<RadioButton*>> RadioButton::groups;
	std::unordered_map<ID, RadioButton*> RadioButton::selecteds;


	RadioButton::RadioButton(GameObject* parent, const std::string& name)
		: CheckBase(parent, name, new Rect())
	{
		SetCheckedImg(ResourceManager::GetImage("UI\\checkedradio.png", true));
		SetUncheckedImg(ResourceManager::GetImage("UI\\uncheckedradio.png", true));
		AddToGroup(this);
	}


	RadioButton::~RadioButton()
	{
	}


	void RadioButton::CopyFrom(GameObject* other)
	{
		SPH_CHECKTYPE(other, RadioButton);

		CheckBase::CopyFrom(other);
		RadioButton* trueOther = (RadioButton*)other;
		SetGroupId(trueOther->groupId);
		SetText(trueOther->text);
	}


	void RadioButton::SetGroupId(ID groupId)
	{
		if (groupId != this->groupId) {
			RemoveFromGroup(this);
			this->groupId = groupId;
			AddToGroup(this);
		}
	}


	const std::string& RadioButton::GetValue()
	{
		return GetValueOf(groupId);
	}


	bool RadioButton::SetValue(const std::string& value)
	{
		std::vector<RadioButton*>& group = groups[groupId];
		for (RadioButton* radio : group) {
			if (radio->GetText() == value) {
				radio->ChangeOnClick();
				return true;
			}
		}
		return false;
	}


	void RadioButton::Clear()
	{
		groups.clear();
		selecteds.clear();
	}


	const std::string& RadioButton::GetValueOf(ID id)
	{
		auto it = selecteds.find(id);
		if (it != selecteds.end() && (*it).second != nullptr) {
			return (*it).second->GetText();
		}

		SPH_ERROR("Cannot find value of id: {0}", id);
		return "";
	}


	void RadioButton::ChangeOnClick()
	{
		std::vector<RadioButton*>& group = groups[groupId];
		for (RadioButton* radio : group) {
			radio->isChecked = false;
		}
		selecteds[groupId] = this;
		isChecked = true;
	}


	void RadioButton::AddToGroup(RadioButton* radio)
	{
		groups[radio->groupId].push_back(radio);
	}


	void RadioButton::RemoveFromGroup(RadioButton* radio)
	{
		std::vector<RadioButton*>& group = groups[radio->groupId];
		auto it = std::find(group.begin(), group.end(), radio);
		if (it != group.end()) {
			groups[radio->groupId].erase(it);
		} else {
			SPH_ERROR("Error in groups map");
		}
		
		if (selecteds[radio->groupId] == radio) {
			radio->isChecked = false;
			if (group.empty()) {
				auto it2 = selecteds.find(radio->groupId);
				if (it2 != selecteds.end()) {
					selecteds.erase(it2);
				} else {
					SPH_ERROR("Error in selecteds map");
				}
			} else {
				selecteds[radio->groupId] = group[0];
				group[0]->isChecked = true;
			}
		}
	}

}
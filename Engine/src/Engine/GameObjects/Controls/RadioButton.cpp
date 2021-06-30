#include "pch.h"
#include "RadioButton.hpp"


namespace Sharpheus {

	std::unordered_map<ID, std::vector<RadioButton*>> RadioButton::groups;
	std::unordered_map<ID, RadioButton*> RadioButton::selecteds;

	ClassInfo RadioButton::classInfo("RadioButton", "radio.png", {
		new UIntProvider<RadioButton>("Group ID", SPH_BIND_GETTER(RadioButton::GetGroupId), SPH_BIND_SETTER(RadioButton::SetGroupId)),
		new ImageProvider<RadioButton>("Checked image", SPH_BIND_GETTER(RadioButton::GetCheckedImg), SPH_BIND_SETTER(RadioButton::SetCheckedImg), SPH_BIND_3(RadioButton::SetCheckedImgFromPath)),
		new ImageProvider<RadioButton>("Unchecked image", SPH_BIND_GETTER(RadioButton::GetUncheckedImg), SPH_BIND_SETTER(RadioButton::SetUncheckedImg), SPH_BIND_3(RadioButton::SetUncheckedImgFromPath)),
		new ColorProvider<RadioButton>("Tint", SPH_BIND_GETTER(RadioButton::GetTint), SPH_BIND_SETTER(RadioButton::SetTint)),
		new StringProvider<RadioButton>("Value", SPH_BIND_GETTER(RadioButton::GetText), SPH_BIND_SETTER(RadioButton::SetText)),
		new BoolProvider<RadioButton>("On right", SPH_BIND_GETTER(RadioButton::IsTextOnRight), SPH_BIND_SETTER(RadioButton::SetTextOnRight)),
		new FontProvider<RadioButton>("Font", SPH_BIND_GETTER(RadioButton::GetFont), SPH_BIND_SETTER(RadioButton::SetFont),
			SPH_BIND_SETTER(RadioButton::SetFontByName), SPH_BIND_3(RadioButton::SetFontByPath), ResourceManager::GetFontTable()),
		new ColorProvider<RadioButton>("Font color", SPH_BIND_GETTER(RadioButton::GetFontColor), SPH_BIND_SETTER(RadioButton::SetFontColor)),
		new UFloatProvider<RadioButton>("Font size", SPH_BIND_GETTER(RadioButton::GetFontSize), SPH_BIND_SETTER(RadioButton::SetFontSize)),
		new FontStyleProvider<RadioButton>("Font style", SPH_BIND_GETTER(RadioButton::GetFontStyle), SPH_BIND_SETTER(RadioButton::SetFontStyle))
		});


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
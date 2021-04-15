#pragma once

#include "CheckBase.hpp"


namespace Sharpheus {

	class SPH_EXPORT RadioButton : public CheckBase
	{
	public:
		RadioButton(GameObject* parent, const std::string& name);
		virtual ~RadioButton();
		virtual void CopyFrom(GameObject* other) override;

		inline ID GetGroupId() { return groupId; }
		void SetGroupId(ID groupId);

		const std::string& GetValue();

		static const std::string& GetValueOf(ID id);

		SPH_DECL_GAMEOBJECT(RadioButton)

	protected:
		ID groupId = 0;

		static std::unordered_map<ID, std::vector<RadioButton*>> groups;
		static std::unordered_map<ID, RadioButton*> selecteds;

		virtual void ChangeOnClick() override;

		static void AddToGroup(RadioButton* radio);
		static void RemoveFromGroup(RadioButton* radio);
	};

}
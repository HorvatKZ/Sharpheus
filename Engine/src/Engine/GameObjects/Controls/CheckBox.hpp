#pragma once

#include "CheckBase.hpp"


namespace Sharpheus {

	class SPH_EXPORT CheckBox : public CheckBase
	{
	public:
		CheckBox(GameObject* parent, const std::string& name);
		virtual ~CheckBox();
		virtual void CopyFrom(GameObject* other) override;

		virtual inline void SetState(bool state) { isChecked = state; };

		SPH_DECL_GAMEOBJECT(CheckBox)

	protected:
		virtual void ChangeOnClick() override;
	};

}
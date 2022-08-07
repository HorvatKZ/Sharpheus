#include "pch.h"
#include "PlaceholderBehavior.hpp"


namespace Sharpheus {

	SPH_START_SPEC_CLASSINFO(PlaceholderBehavior, "CppBehavior", "behaviorcpp.png")
		SPH_PROVIDE_BEHAVIOR("SubType")
	SPH_END_CLASSINFO


	bool PlaceholderBehavior::IsCompatibleWithParent(GameObject* parent)
	{
		return parent != nullptr;
	}

	void PlaceholderBehavior::Init()
	{
	}

}
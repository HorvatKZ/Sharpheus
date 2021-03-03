#include "pch.h"
#include "PlaceholderBehavior.hpp"


namespace Sharpheus {

	ClassInfo PlaceholderBehavior::classInfo("Behavior", "behavior.png", {
		new BehaviorProvider("SubType")
	});


	bool PlaceholderBehavior::IsCompatibleWithParent(GameObject* parent)
	{
		return parent != nullptr;
	}

}
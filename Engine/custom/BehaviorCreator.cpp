#include "pch.h"
#include "BehaviorCreator.hpp"


std::unordered_map<uint32_t, std::string> BehaviorCreator::behaviorNames({
    {1, "DebugBehavior"},
    {2, "PlayerContoller"}
});


Sharpheus::Behavior* BehaviorCreator::Create(uint32_t subType, Sharpheus::PlaceholderBehavior* other)
{
	switch (subType)
	{
		case 1:
			return new DebugBehavior(other);
		case 2:
			return new PlayerController(other);
	}

	SPH_ERROR("BehaviorCreator: Cannot create Behavior from subType {0}", subType);
	return nullptr;
}


Sharpheus::Behavior* BehaviorCreator::Create(uint32_t subType, Sharpheus::GameObject* parent, const std::string& name)
{
	switch (subType)
	{
		case 0:
			return new Sharpheus::PlaceholderBehavior(parent, name);
		case 1:
			return new DebugBehavior(parent, name);
		case 2:
			return new PlayerController(parent, name);
	}

	SPH_ERROR("BehaviorCreator: Cannot create Behavior from subType {0}", subType);
	return nullptr;
}


bool BehaviorCreator::IsCompatibleWithParent(uint32_t subType, Sharpheus::GameObject* parent)
{
	switch (subType)
	{
		case 0:
			return Sharpheus::PlaceholderBehavior::IsCompatibleWithParent(parent);
		case 1:
			return DebugBehavior::IsCompatibleWithParent(parent);
		case 2:
			return PlayerController::IsCompatibleWithParent(parent);
	}

	SPH_ERROR("BehaviorCreator: Unhandled subType {0} at IsCompatibleWithParent call", subType);
	return false;
}

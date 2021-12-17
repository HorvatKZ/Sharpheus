// This file is generated by Sharpheus. Do not edit!
#include "pch.h"
#include "BehaviorCreator.hpp"


std::unordered_map<uint32, std::string> BehaviorCreator::behaviorNames({
	{1, "City"}
});


Sharpheus::Behavior* BehaviorCreator::Create(uint32 subType, Sharpheus::PlaceholderBehavior* other)
{
	SPH_START_BEHAVIORS(subType)
		SPH_CREATE_BEHAVIOR_1(City, 1, other)
	SPH_END_BEHAVIOR_CREATION(subType)
}


Sharpheus::Behavior* BehaviorCreator::Create(uint32 subType, Sharpheus::GameObject* parent, const std::string& name)
{
	SPH_START_BEHAVIORS(subType)
		SPH_CREATE_BEHAVIOR_2(Sharpheus::PlaceholderBehavior, 0, parent, name)
		SPH_CREATE_BEHAVIOR_2(City, 1, parent, name)
	SPH_END_BEHAVIOR_CREATION(subType)
}


bool BehaviorCreator::IsCompatibleWithParent(uint32 subType, Sharpheus::GameObject* parent)
{
	SPH_START_BEHAVIORS(subType)
		SPH_IS_BEHAVIOR_COMP(Sharpheus::PlaceholderBehavior, 0, parent)
		SPH_IS_BEHAVIOR_COMP(City, 1, parent)
	SPH_END_BEHAVIOR_COMP(subType)
}

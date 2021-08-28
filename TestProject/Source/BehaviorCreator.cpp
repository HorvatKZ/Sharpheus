// This file is generated by Sharpheus. Do not edit!
#include "pch.h"
#include "BehaviorCreator.hpp"


std::unordered_map<uint32, std::string> BehaviorCreator::behaviorNames({
	{1, "DebugBehavior"},
	{2, "PlayerContoller"},
	{3, "TriggerTester"},
	{4, "MoueTester"},
	{5, "Controller"}
});


Sharpheus::Behavior* BehaviorCreator::Create(uint32 subType, Sharpheus::PlaceholderBehavior* other)
{
	SPH_START_BEHAVIORS(subType)
		SPH_CREATE_BEHAVIOR_1(DebugBehavior, 1, other)
		SPH_CREATE_BEHAVIOR_1(PlayerController, 2, other)
		SPH_CREATE_BEHAVIOR_1(TriggerTester, 3, other)
		SPH_CREATE_BEHAVIOR_1(MoueTester, 4, other)
		SPH_CREATE_BEHAVIOR_1(Controller, 5, other)
	SPH_END_BEHAVIOR_CREATION(subType)
}


Sharpheus::Behavior* BehaviorCreator::Create(uint32 subType, Sharpheus::GameObject* parent, const std::string& name)
{
	SPH_START_BEHAVIORS(subType)
		SPH_CREATE_BEHAVIOR_2(Sharpheus::PlaceholderBehavior, 0, parent, name)
		SPH_CREATE_BEHAVIOR_2(DebugBehavior, 1, parent, name)
		SPH_CREATE_BEHAVIOR_2(PlayerController, 2, parent, name)
		SPH_CREATE_BEHAVIOR_2(TriggerTester, 3, parent, name)
		SPH_CREATE_BEHAVIOR_2(MoueTester, 4, parent, name)
		SPH_CREATE_BEHAVIOR_2(Controller, 5, parent, name)
	SPH_END_BEHAVIOR_CREATION(subType)
}


bool BehaviorCreator::IsCompatibleWithParent(uint32 subType, Sharpheus::GameObject* parent)
{
	SPH_START_BEHAVIORS(subType)
		SPH_IS_BEHAVIOR_COMP(Sharpheus::PlaceholderBehavior, 0, parent)
		SPH_IS_BEHAVIOR_COMP(DebugBehavior, 1, parent)
		SPH_IS_BEHAVIOR_COMP(PlayerController, 2, parent)
		SPH_IS_BEHAVIOR_COMP(TriggerTester, 3, parent)
		SPH_IS_BEHAVIOR_COMP(MoueTester, 4, parent)
		SPH_IS_BEHAVIOR_COMP(Controller, 5, parent)
	SPH_END_BEHAVIOR_COMP(subType)
}


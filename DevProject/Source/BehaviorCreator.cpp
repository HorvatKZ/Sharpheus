// This file is generated by Sharpheus. Do not edit!
#include "BehaviorCreator.hpp"


const std::unordered_map<uint32, std::string> BehaviorCreator::behaviorNames({
	{3, "DebugBehavior"},
	{4, "PlayerContoller"},
	{5, "TriggerTester"},
	{6, "MoueTester"},
	{7, "Controller"}
});


BehaviorCreator* BehaviorCreator::Instance()
{
	static BehaviorCreator instance;
	return &instance;
}


Sharpheus::Behavior* BehaviorCreator::Create(uint32 subType, Sharpheus::PlaceholderBehavior* other)
{
	SPH_START_BEHAVIORS(subType)
		SPH_CREATE_BEHAVIOR_1(DebugBehavior, 3, other)
		SPH_CREATE_BEHAVIOR_1(PlayerController, 4, other)
		SPH_CREATE_BEHAVIOR_1(TriggerTester, 5, other)
		SPH_CREATE_BEHAVIOR_1(MoueTester, 6, other)
		SPH_CREATE_BEHAVIOR_1(Controller, 7, other)
	SPH_END_BEHAVIOR_CREATION(subType)
}


Sharpheus::Behavior* BehaviorCreator::Create(uint32 subType, Sharpheus::GameObject* parent, const std::string& name)
{
	SPH_START_BEHAVIORS(subType)
		SPH_CREATE_BEHAVIOR_2(Sharpheus::PlaceholderBehavior, 0, parent, name)
		SPH_CREATE_BEHAVIOR_2(Sharpheus::PythonRunnerBehavior, 1, parent, name)
		SPH_CREATE_BEHAVIOR_2(Sharpheus::PythonBehavior, 2, parent, name)
		SPH_CREATE_BEHAVIOR_2(DebugBehavior, 3, parent, name)
		SPH_CREATE_BEHAVIOR_2(PlayerController, 4, parent, name)
		SPH_CREATE_BEHAVIOR_2(TriggerTester, 5, parent, name)
		SPH_CREATE_BEHAVIOR_2(MoueTester, 6, parent, name)
		SPH_CREATE_BEHAVIOR_2(Controller, 7, parent, name)
	SPH_END_BEHAVIOR_CREATION(subType)
}


bool BehaviorCreator::IsCompatibleWithParent(uint32 subType, Sharpheus::GameObject* parent)
{
	SPH_START_BEHAVIORS(subType)
		SPH_IS_BEHAVIOR_COMP(Sharpheus::PlaceholderBehavior, 0, parent)
		SPH_IS_BEHAVIOR_COMP(Sharpheus::PythonRunnerBehavior, 1, parent)
		SPH_IS_BEHAVIOR_COMP(Sharpheus::PythonBehavior, 2, parent)
		SPH_IS_BEHAVIOR_COMP(DebugBehavior, 3, parent)
		SPH_IS_BEHAVIOR_COMP(PlayerController, 4, parent)
		SPH_IS_BEHAVIOR_COMP(TriggerTester, 5, parent)
		SPH_IS_BEHAVIOR_COMP(MoueTester, 6, parent)
		SPH_IS_BEHAVIOR_COMP(Controller, 7, parent)
	SPH_END_BEHAVIOR_COMP(subType)
}


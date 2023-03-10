// This file is generated by Sharpheus. Do not edit!
#include "BehaviorCreator.hpp"


const std::unordered_map<uint32_t, std::string> BehaviorCreator::behaviorNames({
});


BehaviorCreator* BehaviorCreator::Instance()
{
	static BehaviorCreator instance;
	return &instance;
}


Sharpheus::Behavior* BehaviorCreator::Create(uint32_t subType, Sharpheus::PlaceholderBehavior* other)
{
	SPH_START_BEHAVIORS(subType)
	SPH_END_BEHAVIOR_CREATION(subType)
}


Sharpheus::Behavior* BehaviorCreator::Create(uint32_t subType, Sharpheus::GameObject* parent, const std::string& name)
{
	SPH_START_BEHAVIORS(subType)
		SPH_CREATE_BEHAVIOR_2(Sharpheus::PlaceholderBehavior, 0, parent, name)
		SPH_CREATE_BEHAVIOR_2(Sharpheus::PythonRunnerBehavior, 1, parent, name)
		SPH_CREATE_BEHAVIOR_2(Sharpheus::PythonBehavior, 2, parent, name)
	SPH_END_BEHAVIOR_CREATION(subType)
}


bool BehaviorCreator::IsCompatibleWithParent(uint32_t subType, Sharpheus::GameObject* parent)
{
	SPH_START_BEHAVIORS(subType)
		SPH_IS_BEHAVIOR_COMP(Sharpheus::PlaceholderBehavior, 0, parent)
		SPH_IS_BEHAVIOR_COMP(Sharpheus::PythonRunnerBehavior, 1, parent)
		SPH_IS_BEHAVIOR_COMP(Sharpheus::PythonBehavior, 2, parent)
	SPH_END_BEHAVIOR_COMP(subType)
}

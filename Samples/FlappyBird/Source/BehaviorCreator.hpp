// This file is generated by Sharpheus. Do not edit!
#pragma once

#include "pch.h"
#include "Behavior.hpp"
#include "Engine/BehaviorCreatorBase.hpp"
#include "PlaceholderBehavior.hpp"
#include "PythonBehavior.hpp"
#include "CustomMacros.h"

// Custom behaviors
#include "CustomBehaviors/BirdCtrl.hpp"
#include "CustomBehaviors/CamCtrl.hpp"
#include "CustomBehaviors/PipeCtrl.hpp"
#include "CustomBehaviors/MenuCtrl.hpp"

#define SPH_PRESENT_BEHAVIOR(obj) \
	SPH_START_PRESENTING(obj) \
		SPH_PRESENT(PlaceholderBehavior, 0) \
		SPH_PRESENT(PythonRunnerBehavior, 1) \
		SPH_PRESENT(PythonBehavior, 2) \
		SPH_PRESENT(BirdCtrl, 3) \
		SPH_PRESENT(CamCtrl, 4) \
		SPH_PRESENT(PipeCtrl, 5) \
		SPH_PRESENT(MenuCtrl, 6) \
	SPH_END_PRESENTING() \


class SPH_CUSTOM_EXPORT BehaviorCreator : public Sharpheus::BehaviorCreatorBase
{
public:
	Sharpheus::Behavior* Create(uint32 subType, Sharpheus::PlaceholderBehavior* other);
	Sharpheus::Behavior* Create(uint32 subType, Sharpheus::GameObject* parent, const std::string& name);

	bool IsCompatibleWithParent(uint32 subType, Sharpheus::GameObject* parent);

	static BehaviorCreator* Instance();

	static const std::unordered_map<uint32, std::string> behaviorNames;
};


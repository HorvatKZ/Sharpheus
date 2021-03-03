#pragma once

#include "pch.h"
#include "Behavior.hpp"
#include "PlaceholderBehavior.hpp"

#define SPH_START_PRESENTING(obj) { switch (((Behavior*)obj)->GetSubType()) {
#define SPH_END_PRESENTING() default: SPHE_ERROR("Unexpected Behavior type during the creation of presenters"); } }
#define SPH_PRESENT(Class, subType) case subType: CreatePresenterFrom<Class>(provider, y); break;


// Custom behaviors
#include "DebugBehavior.hpp"
#include "PlayerController.hpp"

#define SPH_PRESENT_BEHAVIOR(obj) \
	SPH_START_PRESENTING(obj) \
		SPH_PRESENT(PlaceholderBehavior, 0) \
		SPH_PRESENT(DebugBehavior, 1) \
		SPH_PRESENT(PlayerController, 2) \
	SPH_END_PRESENTING()


class SPH_EXPORT BehaviorCreator
{
public:
	static Sharpheus::Behavior* Create(uint32_t subType, Sharpheus::PlaceholderBehavior* other);
	static Sharpheus::Behavior* Create(uint32_t subType, Sharpheus::GameObject* parent, const std::string& name);

	static bool IsCompatibleWithParent(uint32_t subType, Sharpheus::GameObject* parent);

	static std::unordered_map<uint32_t, std::string> behaviorNames;
};


#pragma once

#include "CustomMacros.h"
#include "Behavior.hpp"


class SPH_CUSTOM_EXPORT DebugBehavior : public Sharpheus::Behavior
{
public:
	DebugBehavior(Sharpheus::Behavior* other);
	DebugBehavior(Sharpheus::GameObject* parent, const std::string& name);
	virtual ~DebugBehavior();

	SPH_DECL_BEHAVIOR(DebugBehavior, 3);

protected:
	void Tick(float deltaTime) override;
};


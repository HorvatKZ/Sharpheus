#pragma once

#include "Behavior.hpp"


class SPH_EXPORT DebugBehavior : public Sharpheus::Behavior
{
public:
	DebugBehavior(Sharpheus::Behavior* other);
	DebugBehavior(Sharpheus::GameObject* parent, const std::string& name);
	virtual ~DebugBehavior();

	SPH_DECL_BEHAVIOR(DebugBehavior, 1);

protected:
	void Tick(float deltaTime) override;
};


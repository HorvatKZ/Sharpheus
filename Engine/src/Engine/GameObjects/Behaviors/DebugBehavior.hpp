#pragma once

#include "Behavior.hpp"


class DebugBehavior : public Sharpheus::Behavior
{
public:
	DebugBehavior(GameObject* parent, const std::string& name);
	virtual ~DebugBehavior();

protected:
	void TickThis(float deltaTime) override;
};


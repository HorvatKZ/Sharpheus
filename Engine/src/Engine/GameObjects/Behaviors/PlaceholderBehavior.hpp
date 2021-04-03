#pragma once

#include "Behavior.hpp"


namespace Sharpheus {

	class PlaceholderBehavior : public Behavior
	{
	public:
		PlaceholderBehavior(GameObject* parent, const std::string& name) : Behavior(parent, name) {}
		virtual ~PlaceholderBehavior() = default;

		SPH_DECL_BEHAVIOR(PlaceholderBehavior, 0);
	};

}
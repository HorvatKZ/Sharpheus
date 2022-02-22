#pragma once

#include "pch.h"


namespace Sharpheus {

	class SPH_EXPORT BehaviorCreatorBase
	{
	public:
		virtual class Behavior* Create(uint32 subType, class PlaceholderBehavior* other) = 0;
		virtual class Behavior* Create(uint32 subType, class GameObject* parent, const std::string& name) = 0;

		virtual bool IsCompatibleWithParent(uint32 subType, class GameObject* parent) = 0;
	};
}
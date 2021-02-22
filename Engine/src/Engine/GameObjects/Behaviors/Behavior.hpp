#pragma once

#include "../GameObject.hpp"


namespace Sharpheus {

	class SPH_EXPORT Behavior : public GameObject
	{
	public:
		Behavior(GameObject* parent, const std::string& name) : GameObject(parent, name) {}
		virtual ~Behavior() = default;

		SPH_DECL_GAMEOBJECT(Behavior);

	protected:
		virtual void Render() override {}
	};
}
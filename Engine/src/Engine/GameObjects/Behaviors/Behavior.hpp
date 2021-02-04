#pragma once

#include "../GameObject.hpp"


namespace Sharpheus {

	class Behavior : public GameObject
	{
	public:
		Behavior(GameObject* parent, const std::string& name, const Transform& trafo = Transform()) : GameObject(parent, name, trafo) {}
		virtual ~Behavior() = default;

		SPH_DECL_GAMEOBJECT(Behavior);

	protected:
		virtual void RenderThis() override {}
	};
}
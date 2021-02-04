#pragma once

#include "GameObject.hpp"


namespace Sharpheus {

	class SPH_EXPORT Collection : public GameObject
	{
	public:
		Collection(GameObject* parent, const std::string& name, const Transform& trafo = Transform()) : GameObject(parent, name, trafo) {}
		virtual ~Collection() = default;

		SPH_DECL_GAMEOBJECT(Collection)

	protected:
		virtual void TickThis(float deltaTime) override {}
		virtual void RenderThis() override {}
	};

}
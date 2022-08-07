#pragma once

#include "../GameObject.hpp"
#include "LocalListeners/RenderableListener.hpp"


namespace Sharpheus {

	class SPH_EXPORT PythonBehavior : public GameObject, public RenderableListener
	{
	public:
		PythonBehavior(GameObject* parent, const std::string& name) : GameObject(parent, name), RenderableListener(GetID()) {}
		virtual ~PythonBehavior() = default;

		SPH_DECL_GAMEOBJECT(PythonBehavior);

	protected:
		bool didExec = false;

		virtual void Tick(float deltaTime) override;
	};

}
#pragma once

#include "../GameObject.hpp"
#include "LocalListeners/RenderableListener.hpp"

namespace pybind11 {
	class object;
}

namespace Sharpheus {

	class SPH_EXPORT PythonBehavior : public GameObject, public RenderableListener
	{
	public:
		PythonBehavior(GameObject* parent, const std::string& name) : GameObject(parent, name), RenderableListener(GetID()) {}
		virtual ~PythonBehavior();

		SPH_DECL_GAMEOBJECT(PythonBehavior);

	protected:
		bool didExec = false;
		pybind11::object* scriptFile = nullptr;

		virtual void Tick(float deltaTime) override;
	};

}
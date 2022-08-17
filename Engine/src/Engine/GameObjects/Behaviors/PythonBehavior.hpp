#pragma once

#include "Behavior.hpp"
#include "LocalListeners/ColliderListener.hpp"
#include "LocalListeners/ControlListener.hpp"
#include "LocalListeners/RenderableListener.hpp"

namespace pybind11 {
	class object;
}

namespace Sharpheus {

	class SPH_EXPORT PythonBehavior : public Behavior, public ColliderListener, public ControlListener, public RenderableListener
	{
	public:
		PythonBehavior(GameObject* parent, const std::string& name) :
			Behavior(parent, name), ColliderListener(GetID()), ControlListener(GetID()), RenderableListener(GetID()) {}
		virtual ~PythonBehavior();

		SPH_DECL_GAMEOBJECT(PythonBehavior);
		SPH_DECL_BEHAVIOR(PythonBehavior, 1);

	protected:
		std::string moduleName = "test";
		pybind11::object* scriptFile = nullptr;
		bool hasTickFunc = false;

		virtual void Tick(float deltaTime) override;
	};

}
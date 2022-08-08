#pragma once

#include "Behavior.hpp"
#include "LocalListeners/RenderableListener.hpp"

namespace pybind11 {
	class object;
}

namespace Sharpheus {

	class SPH_EXPORT PythonBehavior : public Behavior, public RenderableListener
	{
	public:
		PythonBehavior(GameObject* parent, const std::string& name) : Behavior(parent, name), RenderableListener(GetID()) {}
		virtual ~PythonBehavior();

		SPH_DECL_BEHAVIOR(PythonBehavior, 1);

	protected:
		std::string moduleName = "test";
		pybind11::object* scriptFile = nullptr;


		virtual void Tick(float deltaTime) override;
	};

}
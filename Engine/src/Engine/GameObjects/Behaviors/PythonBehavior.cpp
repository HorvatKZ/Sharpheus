#include "pch.h"
#include "PythonBehavior.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"

#include <pybind11/embed.h>

namespace py = pybind11;


namespace Sharpheus {

	SPH_START_CLASSINFO(PythonBehavior, "behaviorpy.png")
	SPH_END_CLASSINFO


	void PythonBehavior::Tick(float deltaTime)
	{
		if (!didExec) {
			
			SubscribeForRender(level, "HUD", [] {
				try {
					py::scoped_interpreter guard{};
					py::eval_file(ResourceManager::GetScriptPath("test"));
				}
				catch (py::error_already_set& e) {
					SPH_ERROR(e.what());
				}
			});
				
			didExec = true;
			
		}
	}

}
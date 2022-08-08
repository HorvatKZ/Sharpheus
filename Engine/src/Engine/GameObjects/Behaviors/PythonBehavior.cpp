#include "pch.h"
#include "PythonBehavior.hpp"
#include "Engine/FileUtils/OSPaths.hpp"
#include "Engine/PythonInterface/PythonInterface.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"
#include <pybind11/pybind11.h>

namespace py = pybind11;


namespace Sharpheus {

	SPH_START_CLASSINFO(PythonBehavior, "behaviorpy.png")
	SPH_END_CLASSINFO


	PythonBehavior::~PythonBehavior()
	{
		PythonInterface::Exec("Freeing module: " + moduleName, [&] {
			delete scriptFile;
		});
	}


	bool PythonBehavior::IsCompatibleWithParent(GameObject* parent)
	{
		return parent != nullptr;
	}


	void PythonBehavior::Init()
	{
		scriptFile = PythonInterface::Import(moduleName);

		bool hasRender = false;
		PythonInterface::Exec("Check if " + moduleName + " has render func", [&] {
			hasRender = py::hasattr(*scriptFile, "render");
		});

		if (hasRender) {
			SubscribeForRender(level, "Default", [&] {
				PythonInterface::Exec(moduleName + ".render()", [&] {
					auto func = scriptFile->attr("render")();
				});
			});
		}
	}


	void PythonBehavior::Tick(float deltaTime)
	{
	}

}
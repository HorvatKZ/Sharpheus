#include "pch.h"
#include "PythonBehavior.hpp"
#include "Engine/FileUtils/OSPaths.hpp"
#include "Engine/PythonInterface/PythonInterface.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"
#include "Engine/Level.hpp"
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
		
		if (SPH_VERIFY_0(scriptFile != nullptr)) {
			bool hasRenderFunc = false;
			std::string renderLayer = level->GetLayerNames()->front();
			PythonInterface::Exec("Check functions of " + moduleName, [&] {
				hasRenderFunc = py::hasattr(*scriptFile, "render");
				hasTickFunc = py::hasattr(*scriptFile, "tick");
				if (py::hasattr(*scriptFile, "render_layer")) {
					renderLayer = scriptFile->attr("render_layer").cast<std::string>();
				}
			});

			if (hasRenderFunc) {
				SubscribeForRender(level, renderLayer, [&] {
					PythonInterface::Exec(moduleName + ".render()", [&] {
						scriptFile->attr("render")();
					});
				});
			}
		}
	}


	void PythonBehavior::Tick(float deltaTime)
	{
		if (hasTickFunc) {
			PythonInterface::Exec(moduleName + ".tick()", [&] {
				scriptFile->attr("tick")(deltaTime);
			});
		}
	}

}
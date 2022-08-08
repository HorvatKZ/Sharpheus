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
		delete scriptFile;
	}


	void PythonBehavior::Tick(float deltaTime)
	{
		if (!didExec) {
			PythonInterface::Exec([&] {
				auto sys = py::module::import("sys");
				sys.attr("path").cast<py::list>().insert(0, ResourceManager::GetScriptsRoot());
				sys.attr("path").cast<py::list>().insert(0, OSPaths::Get(OSPaths::Folder::EXEC_FOLDER));
				scriptFile = new py::object(py::module::import("test"));
			});
			
			SubscribeForRender(level, "Default", [&] {
				PythonInterface::Exec([&] {
					auto func = scriptFile->attr("render");
					func();
				});
			});
				
			didExec = true;
			
		}
	}

}
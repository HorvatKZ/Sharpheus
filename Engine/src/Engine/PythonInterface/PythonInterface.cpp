#include "pch.h"
#include "PythonInterface.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"
#include "Engine/FileUtils/OSPaths.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>


namespace Sharpheus {

	bool PythonInterface::interpreter_inited = false;
	std::unordered_map<std::string, py::object*> PythonInterface::loadedModules;


	void PythonInterface::Init()
	{
	}

	void PythonInterface::Clear()
	{
		if (interpreter_inited) {
			for (auto it = loadedModules.begin(); it != loadedModules.end(); ++it) {
				SPH_PYEXEC("Freeing module " + it->first, [&] { delete it->second; });
			}

			SPH_INFO("Finalize Python interpreter");
			py::finalize_interpreter();
		}
	}

	py::object* PythonInterface::Import(const std::string& moduleName)
	{
		auto it = loadedModules.find(moduleName);
		if (it != loadedModules.end()) {
			return (*it).second;
		}

		py::object* moduleFile = nullptr;
		SPH_PYEXEC("Importing module " + moduleName, [&moduleFile, moduleName] {
			moduleFile = new py::object(py::module::import(moduleName.c_str()));
		});
		loadedModules[moduleName] = moduleFile;
		return moduleFile;
	}


	void PythonInterface::Exec(
#ifdef SPH_DO_LOGGING
		const std::string& info,
#endif
		const std::function<void()>& func)
	{
		try {
			if (!interpreter_inited) {
				SPH_INFO("Init Python interpreter");
				py::initialize_interpreter();
				auto sys = py::module::import("sys");
				sys.attr("path").cast<py::list>().insert(0, ResourceManager::GetScriptsRoot());
				sys.attr("path").cast<py::list>().insert(0, OSPaths::Get(OSPaths::Folder::EXEC_FOLDER));
				interpreter_inited = true;
			}
#ifdef SPH_DO_LOGGING
			SPH_LOG(info);
#endif
			func();
		} catch (py::error_already_set& e) {
			SPH_ERROR("PythonInterface::Exec error\n" + std::string(e.what()));
		}
	}

}

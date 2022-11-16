#include "pch.h"
#include "PythonInterface.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"
#include "Engine/FileUtils/OSPaths.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>


namespace Sharpheus {

	const std::string PythonInterface::moduleName = "Engine";
	bool PythonInterface::interpreter_inited = false;
	std::unordered_map<std::string, py::object*> PythonInterface::loadedModules;


	void PythonInterface::Init()
	{
	}

	void PythonInterface::Clear()
	{
		if (interpreter_inited) {
			for (auto it = loadedModules.begin(); it != loadedModules.end(); ++it) {
				PythonInterface::Exec("Freeing module " + it->first, [&] { delete it->second; });
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
		if (PythonInterface::Exec("Importing module " + moduleName, [&moduleFile, moduleName] {
			moduleFile = new py::object(py::module::import(moduleName.c_str()));
			})) {
			loadedModules[moduleName] = moduleFile;
			return moduleFile;
		}

		return nullptr;
	}


	bool PythonInterface::Exec(const std::string& info, const std::function<void()>& func)
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
			SPH_LOG(info);
			func();
		} catch (py::error_already_set& e) {
			SPH_ERROR("PythonInterface::Exec error\n" + std::string(e.what()));
			return false;
		}
		return true;
	}


	bool PythonInterface::Precompile(const std::string& pyFile, const std::string& pycResult)
	{
		return PythonInterface::Exec("Precompiling " + pyFile + " to " + pycResult, [&] {
			py::object* py_compile = PythonInterface::Import("py_compile");
			py_compile->attr("compile")(pyFile, pycResult, py::none(), true);
		});
	}

}

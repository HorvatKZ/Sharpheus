#include "pch.h"
#include "PythonInterface.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"
#include "Engine/FileUtils/OSPaths.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>


namespace py = ::pybind11;


namespace Sharpheus {

	bool PythonInterface::interpreter_inited = false;

	void PythonInterface::Init()
	{
	}

	void PythonInterface::Clear()
	{
		if (interpreter_inited) {
			py::finalize_interpreter();
		}
	}

	py::object* PythonInterface::Import(const std::string& moduleName)
	{
		py::object* moduleFile = nullptr;
		PythonInterface::Exec("Importing " + moduleName, [&moduleFile, moduleName] {
			moduleFile = new py::object(py::module::import(moduleName.c_str()));
		});
		return moduleFile;
	}


	void PythonInterface::Exec(const std::string& info, const std::function<void()>& func)
	{
		try {
			if (!interpreter_inited) {
				py::initialize_interpreter();
				auto sys = py::module::import("sys");
				sys.attr("path").cast<py::list>().insert(0, ResourceManager::GetScriptsRoot());
				sys.attr("path").cast<py::list>().insert(0, OSPaths::Get(OSPaths::Folder::EXEC_FOLDER));
				interpreter_inited = true;
			}
			func();
		} catch (py::error_already_set& e) {
			SPH_ERROR("PythonInterface::Exec " + info + " " + e.what());
		}
	}

}

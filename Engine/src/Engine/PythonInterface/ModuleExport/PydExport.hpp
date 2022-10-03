#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include "sph_functional.h"


namespace py = ::pybind11;

using namespace ::pybind11::literals;


namespace Sharpheus {

	void Export_Logger(py::module_& handle);
	void Export_Geometry(py::module_& handle);
	void Export_Renderer(py::module_& handle);
	void Export_ResourceManager(py::module_& handle);
	void Export_FileUtils(py::module_& handle);
	void Export_Events(py::module_& handle);
	void Export_GameObjects(py::module_& handle);
	void Export_EngineCore(py::module_& handle);

	byte kwargs2FontStyle(const py::kwargs& kwargs);

}
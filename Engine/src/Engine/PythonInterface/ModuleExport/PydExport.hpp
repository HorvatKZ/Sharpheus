#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>


namespace py = ::pybind11;

using namespace ::pybind11::literals;


namespace Sharpheus {

	void Export_Logger(py::module_& handle);
	void Export_Geometry(py::module_& handle);
	void Export_Renderer(py::module_& handle);
	void Export_ResourceManager(py::module_& handle);

}
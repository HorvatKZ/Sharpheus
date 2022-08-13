#include "pch.h"
#include "PydExport.hpp"


namespace Sharpheus {

	void Export_Logger(py::module_& handle)
	{
		py::module_ m = handle.def_submodule("Logger");
		m.doc() = "Engine logger functions for Sharpheus";

		m.def("log", [](const std::string& msg) { SPH_LOG(msg); }, "msg"_a);
		m.def("info", [](const std::string& msg) { SPH_INFO(msg); }, "msg"_a);
		m.def("warn", [](const std::string& msg) { SPH_WARN(msg); }, "msg"_a);
		m.def("error", [](const std::string& msg) { SPH_ERROR(msg); }, "msg"_a);
		m.def("fatal", [](const std::string& msg) { SPH_FATAL(msg); }, "msg"_a);
	}

}
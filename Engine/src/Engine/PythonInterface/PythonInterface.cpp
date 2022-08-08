#include "pch.h"
#include "PythonInterface.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"
#include "Engine/FileUtils/OSPaths.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>


namespace py = ::pybind11;

using namespace ::pybind11::literals;


namespace Sharpheus {

	void Export_Point(py::module_& handle)
	{
		py::class_<Point>(handle, "Point")
			.def_readwrite("x", &Point::x)
			.def_readwrite("y", &Point::y)

			.def(py::init<float, float>(), "x"_a = 0.f, "y"_a = 0.f)

			.def(py::self + py::self)
			.def(py::self - py::self)
			.def(py::self * py::self)
			.def(py::self * float())
			.def(float() * py::self)
			.def(py::self / float())
			.def(float() / py::self)
			.def(py::self += py::self)
			.def(py::self -= py::self)
			.def(py::self *= float())
			.def(py::self /= float())
			.def(py::self == py::self)
			.def(py::self != py::self)

			.def("length", &Point::Length)
			.def("length_squared", &Point::LengthSquared)
			.def("distance", &Point::Distance, "other"_a)
			.def("distance_squared", &Point::DistanceSquared, "other"_a)
			.def("normalize", &Point::Normalize)

			.def("rotate", &Point::Rotate, "angle"_a)
			.def("get_angle", &Point::GetAngle)
			.def("get_angle_with", &Point::GetAngleWith, "other"_a)

			.def_readonly_static("ZERO", &Point::Zero)
			.def_readonly_static("UP", &Point::Up)
			.def_readonly_static("DOWN", &Point::Down)
			.def_readonly_static("LEFT", &Point::Left)
			.def_readonly_static("RIGHT", &Point::Right)

			.def_static("get_unit", &Point::GetUnit, "angle"_a);
	}


	void Export_Trafo(py::module_& handle)
	{
		py::class_<Transform>(handle, "Transform")
			.def_readwrite("pos", &Transform::pos)
			.def_readwrite("scale", &Transform::scale)
			.def_readwrite("rot", &Transform::rot)

			.def(py::init<const Point&, const Point&, float>(), "pos"_a = Point(), "scale"_a = Point(1.f, 1.f), "rot"_a = 0)

			.def(py::self == py::self)
			.def(py::self != py::self)
			.def(py::self + py::self)

			.def("substract_first", &Transform::SubstractFirst, "other"_a)
			.def("substract_second", &Transform::SubstractSecond, "other"_a);
	}


	void Export_Color(py::module_& handle)
	{
		py::class_<Color>(handle, "Color")
			.def_readwrite("r", &Color::r)
			.def_readwrite("g", &Color::g)
			.def_readwrite("b", &Color::b)
			.def_readwrite("a", &Color::a)

			.def(py::init<>())
			.def(py::init<byte, byte, byte, byte>(), "r"_a, "g"_a, "b"_a, "a"_a = 255)

			.def("get_red", &Color::GetRed)
			.def("get_green", &Color::GetGreen)
			.def("get_blue", &Color::GetBlue)
			.def("get_alpha", &Color::GetAlpha)

			.def("make_opaque", &Color::MakeOpaque, "percent"_a)

			.def_readonly_static("BLACK", &Color::Black)
			.def_readonly_static("RED", &Color::Red)
			.def_readonly_static("GREEN", &Color::Green)
			.def_readonly_static("BLUE", &Color::Blue)
			.def_readonly_static("YELLOW", &Color::Yellow)
			.def_readonly_static("CYAN", &Color::Cyan)
			.def_readonly_static("MAGENTA", &Color::Magenta)
			.def_readonly_static("WHITE", &Color::White)
			.def_readonly_static("TRANSPARENT", &Color::Transparent);
	}


	void Export_Logger(py::module_& handle)
	{
		py::module_ m = handle.def_submodule("Logger");
		m.def("log", [](const std::string& msg) { SPH_LOG(msg); }, "msg"_a);
		m.def("info", [](const std::string& msg) { SPH_INFO(msg); }, "msg"_a);
		m.def("warn", [](const std::string& msg) { SPH_WARN(msg); }, "msg"_a);
		m.def("error", [](const std::string& msg) { SPH_ERROR(msg); }, "msg"_a);
		m.def("fatal", [](const std::string& msg) { SPH_FATAL(msg); }, "msg"_a);
	}


	void Export_Renderer(py::module_& handle)
	{
		// TODO - finish
		py::module_ m = handle.def_submodule("Renderer");
		m.def("get_invalid_texture", &Renderer::GetInvalidTexture);
		m.def("draw_circle", [](const std::vector<Point>& coords, const Color& tint) { Renderer::DrawCircle(coords.data(), tint); }, "coords"_a, "tint"_a);
	}


	PYBIND11_MODULE(Engine, handle) {
		handle.doc() = "Sharpheus Game Engine Python Interface [using pybind11]";

		Export_Logger(handle);

		Export_Point(handle);
		Export_Color(handle);
		Export_Trafo(handle);

		Export_Renderer(handle);
	}

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

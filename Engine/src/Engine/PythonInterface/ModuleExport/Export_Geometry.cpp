#include "pch.h"
#include "PydExport.hpp"


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

			.def_static("get_unit", &Point::GetUnit, "angle"_a)

			.def("__repr__", [](const Point& p) { return "<Sharpheus.Point (" + std::to_string(p.x) + ", " + std::to_string(p.y) + ")>"; });
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
			.def("substract_second", &Transform::SubstractSecond, "other"_a)

			.def("__repr__", [](const Transform& t) { return "<Sharpheus.Transform (pos = (" + std::to_string(t.pos.x) + ", " + std::to_string(t.pos.y) +
				"), scale = (" + std::to_string(t.scale.x) + ", " + std::to_string(t.scale.y) + "), rot = " + std::to_string(t.rot) + ")>"; });
	}


	void Export_Shapes(py::module_& handle)
	{
		// TODO shapes
	}


	void Export_Geometry(py::module_& handle)
	{
		Export_Point(handle);
		Export_Trafo(handle);
		Export_Shapes(handle);
	}

}
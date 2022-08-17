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
		py::class_<Shape> shap(handle, "Shape");

		py::enum_<Shape::Type>(shap, "Type")
			.value("OVAL", Shape::Type::OVAL)
			.value("RECT", Shape::Type::RECT)
			.value("CAPSULE", Shape::Type::CAPSULE);

		py::class_<Shape::Intersection>(shap, "Intersection")
			.def(py::init<const Point&, const Point&, float>(), "contact"_a, "normal"_a, "depth"_a)
			.def_readwrite("contact", &Shape::Intersection::contact)
			.def_readwrite("normal", &Shape::Intersection::normal)
			.def_readwrite("depth", &Shape::Intersection::depth);

		shap.def_property("pos", &Shape::GetPos, &Shape::SetPos)
			.def_property("dim", &Shape::GetDim, &Shape::SetDim)
			.def_property("rot", &Shape::GetRot, &Shape::SetRot)
			.def_property_readonly("x_axis", &Shape::GetXAxis)
			.def_property_readonly("y_axis", &Shape::GetXAxis)
			.def_property_readonly("corners", [](Shape& shap) {
				std::vector<Point> result;
				Point* corners = shap.GetCorners();
				for (uint8 i = 0; i < 4; ++i) {
					result.push_back(corners[i]);
				}
				return result;
			})
			.def_property_readonly("sat_corners", [](Shape& shap) {
				std::vector<Point> result;
				Point* corners = shap.GetSATCorners();
				for (uint8 i = 0; i < shap.GetSATCornerNum(); ++i) {
					result.push_back(corners[i]);
				}
				return result;
			})

			.def("type", &Shape::GetType)
			.def("is_inside", &Shape::IsInside, "pos"_a)
			.def("get_local_perpendicular_at", &Shape::GetLocalPerpendicularAt, "surface_point"_a)
			.def("get_local_closest_to", &Shape::GetLocalClosestTo, "point"_a)
			.def("get_priority", &Shape::GetPriority)

			.def("get_sat_corner_num", &Shape::GetSATCornerNum)
			.def("is_sat_symmetrical", &Shape::IsSATSymmetrical)

			.def("is_too_far_from", py::overload_cast<const Shape&>(&Shape::IsTooFarFrom), "other"_a)

			.def("force_refresh", &Shape::ForceRefresh)

			.def("get_intersection_with", &Shape::GetIntersectionWith);

		py::class_<Rect>(handle, "Rect")
			.def(py::init<const Point&, const Point&, float>(), "pos"_a, "dim"_a, "rot"_a)
			
			.def("__repr__", [](Rect& r) { return "<Sharpheus.Rect (pos = (" + std::to_string(r.GetPos().x) + ", " + std::to_string(r.GetPos().y) +
				"), dim = (" + std::to_string(r.GetDim().x) + ", " + std::to_string(r.GetDim().y) + "), rot = " + std::to_string(r.GetRot()) + ")>"; });;

		py::class_<Oval>(handle, "Oval")
			.def(py::init<const Point&, const Point&, float>(), "pos"_a, "dim"_a, "rot"_a)
			.def_property_readonly("is_circle", &Oval::IsCircle)
			
			.def("__repr__", [](Oval& o) { return "<Sharpheus.Oval (pos = (" + std::to_string(o.GetPos().x) + ", " + std::to_string(o.GetPos().y) +
				"), dim = (" + std::to_string(o.GetDim().x) + ", " + std::to_string(o.GetDim().y) + "), rot = " + std::to_string(o.GetRot()) + ")>"; });

		py::class_<Capsule>(handle, "Capsule")
			.def(py::init<const Point&, const Point&, float>(), "pos"_a, "dim"_a, "rot"_a)
			.def_property("dome_height", &Capsule::GetDomeHeight, &Capsule::SetDomeHeight)
			.def_property_readonly("inner_corners", [](Capsule& caps) {
				std::vector<Point> result;
				Point* corners = caps.GetInnerCorners();
				for (uint8 i = 0; i < 4; ++i) {
					result.push_back(corners[i]);
				}
				return result;
			})
			
			.def("__repr__", [](Capsule& c) { return "<Sharpheus.Capsule (pos = (" + std::to_string(c.GetPos().x) + ", " + std::to_string(c.GetPos().y) +
				"), dim = (" + std::to_string(c.GetDim().x) + ", " + std::to_string(c.GetDim().y) + "), rot = " + std::to_string(c.GetRot()) + 
				+ ", dome_height = " + std::to_string(c.GetDomeHeight()) + ")>"; });
	}


	void Export_Geometry(py::module_& handle)
	{
		Export_Point(handle);
		Export_Trafo(handle);
		Export_Shapes(handle);
	}

}
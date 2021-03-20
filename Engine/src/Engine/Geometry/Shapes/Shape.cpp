#include "pch.h"
#include "Shape.hpp"


namespace Sharpheus {

	Shape::Shape()
		: rot(0.f), furthest(0.f), xAxis(1, 0), yAxis(0, 1)
	{
		CheckCorners();
	}


	Shape::Shape(const Point& pos, const Point& dim, float rot)
		: pos(pos), dim(dim), rot(rot), furthest(dim.x > dim.y ? dim.x : dim.y)
	{
		RecalcAxes();
		CheckCorners();
	}


	Shape::~Shape()
	{
	}


	void Shape::ForceRefresh()
	{
		needsToRecalc = true;
		CheckCorners();
	}


	void Shape::RecalcAxes()
	{
		xAxis = Point::GetUnit(rot);
		yAxis = Point::GetUnit(rot - 90);
	}


	void Shape::CheckCorners()
	{
		if (needsToRecalc) {
			Point height = yAxis * dim.y;
			Point width = xAxis * dim.x;
			corners[0] = pos - height - width;
			corners[1] = pos - height + width;
			corners[2] = pos + height + width;
			corners[3] = pos + height - width;
			needsToRecalc = false;
		}
	}


	Point Shape::GetRelativePos(const Point& p)
	{
		Point diff = p - pos;
		return Point(diff * xAxis, diff * yAxis);
	}

}
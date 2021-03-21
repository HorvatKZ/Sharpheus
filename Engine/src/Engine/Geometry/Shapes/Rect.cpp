#include "pch.h"
#include "Rect.hpp"


namespace Sharpheus {

	Rect::Rect(const Point& pos, const Point& dim, float rot)
		: Shape(pos, dim, rot)
	{
	}


	Rect::~Rect()
	{
	}


	bool Rect::IsInside(const Point& p)
	{
		Point relP = GetRelativePos(p);
		return -dim.x <= relP.x && relP.x <= dim.x && -dim.y <= relP.y && relP.y <= dim.y;
	}


	Point Rect::GetLocalPerpendicularAt(const Point& surfaceP)
	{
		Point scaled(surfaceP.x * dim.y, surfaceP.y * dim.x);
		if (scaled.x > scaled.y) {
			if (scaled.x > -scaled.y) {
				return Point(1, 0);
			}
			else {
				return Point(0, -1);
			}
		}
		else {
			if (scaled.x > -scaled.y) {
				return Point(0, 1);
			}
			else {
				return Point(-1, 0);
			}
		}
	}


	Shape::Intersection Rect::GetIntersectionWith(Oval* other)
	{
		Intersection inter = other->GetIntersectionWith((Shape*)this);
		inter.normal *= -1;
		return inter;
	}

	Shape::Intersection Rect::GetIntersectionWith(Rect* other)
	{
		return Intersection();
	}

	Shape::Intersection Rect::GetIntersectionWith(Capsule* other)
	{
		return Intersection();
	}

}
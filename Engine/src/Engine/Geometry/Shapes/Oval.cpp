#include "pch.h"
#include "Oval.hpp"


namespace Sharpheus {

	Oval::Oval(const Point& pos, const Point& dim, float rot)
		: Shape(pos, dim, rot)
	{
	}


	Oval::~Oval()
	{
	}


	bool Oval::IsInside(const Point& p)
	{
		Point relP = GetRelativePos(p);
		Point unscaledP(relP.x / dim.x, relP.y / dim.y);
		return unscaledP.LengthSquared() <= 1.f;
	}


	CollData Oval::GetCollWith(const Oval& other)
	{
		return CollData();
	}

	CollData Oval::GetCollWith(const Rect& other)
	{
		return CollData();
	}

	CollData Oval::GetCollWith(const Capsule& other)
	{
		return CollData();
	}

}
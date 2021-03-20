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


	CollData Rect::GetCollWith(const Oval& other)
	{
		return CollData();
	}


	CollData Rect::GetCollWith(const Rect& other)
	{
		return CollData();
	}


	CollData Rect::GetCollWith(const Capsule& other)
	{
		return CollData();
	}

}
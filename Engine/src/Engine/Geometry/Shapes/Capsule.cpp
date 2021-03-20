#include "pch.h"
#include "Capsule.hpp"


namespace Sharpheus {

	Capsule::Capsule(const Point& pos, const Point& dim, float rot)
		: Shape(pos, dim, rot), domeHeight(dim.x)
	{
	}


	Capsule::~Capsule()
	{
	}


	bool Capsule::IsInside(const Point& p)
	{
		Point relP = GetRelativePos(p);

		if (relP.y > dim.y || relP.y < -dim.y) {
			return false;
		}

		if (relP.y > dim.y - domeHeight) {
			return (relP.x * relP.x) / (dim.x * dim.x) + ((relP.y - dim.y + domeHeight) * (relP.y - dim.y + domeHeight)) / (domeHeight * domeHeight) <= 1.f;
		}

		if (relP.y < -dim.y + domeHeight) {
			return (relP.x * relP.x) / (dim.x * dim.x) + ((relP.y + dim.y - domeHeight) * (relP.y + dim.y - domeHeight)) / (domeHeight * domeHeight) <= 1.f;
		}

		return -dim.x <= relP.x && relP.x <= dim.x;
	}


	void Capsule::ForceRefresh()
	{
		Shape::ForceRefresh();

		needsToRecalcInner = true;
		CheckInnerCorners();
	}


	CollData Capsule::GetCollWith(const Oval& other)
	{
		return CollData();
	}


	CollData Capsule::GetCollWith(const Rect& other)
	{
		return CollData();
	}


	CollData Capsule::GetCollWith(const Capsule& other)
	{
		return CollData();
	}


	void Capsule::CheckInnerCorners()
	{
		if (needsToRecalcInner) {
			Point height = yAxis * (dim.y - domeHeight);
			Point width = xAxis * dim.x;
			innerCorners[0] = pos - height - width;
			innerCorners[1] = pos - height + width;
			innerCorners[2] = pos + height + width;
			innerCorners[3] = pos + height - width;
			needsToRecalcInner = false;
		}
	}

}
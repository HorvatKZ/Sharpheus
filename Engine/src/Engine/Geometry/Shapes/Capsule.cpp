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


	Point Capsule::GetLocalPerpendicularAt(const Point& surfaceP)
	{
		if (surfaceP.y < -dim.y + domeHeight) { // Upper half circle
			return Point(surfaceP.x * domeHeight / dim.x, (surfaceP.y + dim.y - domeHeight) * dim.x / domeHeight);
		}
		else if (surfaceP.y > dim.y - domeHeight) { // Lower half circle
			return Point(surfaceP.x * domeHeight / dim.x, (surfaceP.y - dim.y + domeHeight) * dim.x / domeHeight);
		}
		else if (surfaceP.x > 0) {
			return Point(1, 0);
		}
		else {
			return Point(-1, 0);
		}
	}


	Point Capsule::GetLocalClosestTo(const Point& p)
	{
		if (p.y < -dim.y + domeHeight) { // Upper half circle
			Point toCenter(p.x, p.y + dim.y - domeHeight);
			toCenter = toCenter.Normalize();
			return Point(toCenter.x * dim.x, toCenter.y * domeHeight - dim.y + domeHeight);
		}
		else if (p.y > dim.y - domeHeight) { // Lower half circle
			Point toCenter(p.x, p.y - dim.y + domeHeight);
			toCenter = toCenter.Normalize();
			return Point(toCenter.x * dim.x, toCenter.y * domeHeight + dim.y - domeHeight);
		}
		else if (p.x > 0) {
			return Point(dim.x, p.y);
		}
		else {
			return Point(-dim.x, p.y);
		}
	}


	void Capsule::ForceRefresh()
	{
		Shape::ForceRefresh();

		needsToRecalcInner = true;
		CheckInnerCorners();
	}


	void Capsule::CheckCorners()
	{
		if (needsToRecalc) {
			Point smallHeight = yAxis * (dim.y - domeHeight * 0.66f);
			Point smallWidth = xAxis * dim.x * 0.33f;
			Point height = yAxis * dim.y;
			Point width = xAxis * dim.x;
			satCorners[0] = pos - height - smallWidth;
			satCorners[1] = pos - height + smallWidth;
			satCorners[2] = pos - smallHeight + width;
			satCorners[3] = pos + smallHeight + width;
			satCorners[4] = pos + height + smallWidth;
			satCorners[5] = pos + height - smallWidth;
			satCorners[6] = pos + smallHeight - width;
			satCorners[7] = pos - smallHeight - width;
		}

		Shape::CheckCorners();
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


	void Capsule::UpdateFurthest()
	{
		furthest = dim.y;
	}

}
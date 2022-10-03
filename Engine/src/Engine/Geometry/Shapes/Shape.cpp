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


	Shape::Intersection Shape::GetIntersectionWith(Shape* other)
	{
		if (other->GetPriority() > GetPriority()) {
			Intersection inter = other->GetIntersectionWith(this);
			inter.normal *= -1;
			return inter;
		}

		IntersectionData usedData = GetIntersectionDataWithSAT(other);
		if (usedData.smallestContact == -1.f) {
			return Intersection();
		}

		Intersection inter;
		inter.normal = usedData.normal;
		if (usedData.fromOther) {
			inter.normal *= -1;
		}
		inter.depth = usedData.smallestContact / 2;
		Point mostInside = usedData.use2 ? (usedData.contactPoints[0] + usedData.contactPoints[1]) / 2 : usedData.contactPoints[0];
		inter.contact = mostInside + usedData.normal * inter.depth;
		return inter;
	}


	void Shape::RecalcAxes()
	{
		xAxis = Point::GetUnit(rot);
		yAxis = Point::GetUnit(rot - 90);
	}


	void Shape::CheckCorners()
	{
		if (needsToRecalc) {
			Point height = yAxis * realDim.y;
			Point width = xAxis * realDim.x;
			needToMirrorX = (realDim.x * realDim.y) < 0;
			corners[needToMirrorX ? 1 : 0] = pos - height - width;
			corners[needToMirrorX ? 0 : 1] = pos - height + width;
			corners[needToMirrorX ? 3 : 2] = pos + height + width;
			corners[needToMirrorX ? 2 : 3] = pos + height - width;
			needsToRecalc = false;
		}
	}


	Point Shape::GetRelativePos(const Point& p)
	{
		Point diff = p - pos;
		return Point(diff * xAxis, diff * yAxis);
	}


	Shape::IntersectionData Shape::GetIntersectionDataWithSAT(Shape* other)
	{
		IntersectionData thisData = GetOneWayIntersectionDataWithSAT(other);
		IntersectionData otherData = other->GetOneWayIntersectionDataWithSAT(this);
		IntersectionData usedData;
		bool otherIsUsed = false;

		if (thisData.smallestContact == -1.f || otherData.smallestContact == -1.f) {
			return IntersectionData();
		}
		
		bool useOther = thisData.smallestContact > otherData.smallestContact;
		usedData = useOther ? otherData : thisData;
		usedData.fromOther = useOther;

		return usedData;
	}


	Shape::IntersectionData Shape::GetOneWayIntersectionDataWithSAT(Shape* other)
	{
		Point* thisCorners = GetSATCorners();
		Point* otherCorners = other->GetSATCorners();
		uint8 thisCornerNum = GetSATCornerNum();
		uint8 otherCornerNum = other->GetSATCornerNum();

		IntersectionData data;
		float bestThisProjected;
		for (uint8 i = 0; i < (IsSATSymmetrical() ? thisCornerNum / 2 : thisCornerNum); ++i) {
			Point normal(thisCorners[(i + 1) % thisCornerNum].y - thisCorners[i].y, thisCorners[i].x - thisCorners[(i + 1) % thisCornerNum].x);
			normal = normal.Normalize();
			float thisMin = thisCorners[0] * normal, otherMin = otherCorners[0] * normal;
			float thisMax = thisMin, otherMax = otherMin;
			for (uint8 j = 1; j < thisCornerNum; ++j) {
				float projected = thisCorners[j] * normal;
				if (projected < thisMin) {
					thisMin = projected;
				}
				if (projected > thisMax) {
					thisMax = projected;
				}
			}
			for (uint8 j = 1; j < otherCornerNum; ++j) {
				float projected = otherCorners[j] * normal;
				if (projected < otherMin) {
					otherMin = projected;
				}
				if (projected > otherMax) {
					otherMax = projected;
				}
			}

			if (thisMax < otherMin || otherMax < thisMin) {
				return IntersectionData();
			}

			float thisLeft = thisMax - otherMin;
			float otherLeft = otherMax - thisMin;
			float currContact;
			float currRelevantProjected;
			if (thisLeft < otherLeft) {
				currContact = thisLeft;
				currRelevantProjected = thisMax;
			}
			else {
				currContact = otherLeft;
				currRelevantProjected = -thisMin;
				normal *= -1;
			}

			if (data.smallestContact == -1.f || currContact < data.smallestContact) {
				data.smallestContact = currContact;
				data.normal = normal;
				bestThisProjected = currRelevantProjected;
			}
		}

		float tolerance = 0.01f;
		uint8 found = 0;
		for (uint8 i = 0; i < thisCornerNum; ++i) {
			float projected = thisCorners[i] * data.normal;
			if (abs(projected - bestThisProjected) < tolerance) {
				data.contactPoints[found++] = thisCorners[i];
			}
		}
		data.use2 = found == 2;
		return data;
	}

}
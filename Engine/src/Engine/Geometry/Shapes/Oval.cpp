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
		Point unscaledP(relP.x * relP.x / (dim.x * dim.x), relP.y * relP.y / (dim.y * dim.y));
		return unscaledP.LengthSquared() <= 1.f;
	}


	Point Oval::GetLocalPerpendicularAt(const Point& surfaceP)
	{
		if (IsCircle()) {
			return surfaceP;
		}

		return Point(surfaceP.x * dim.y / dim.x, surfaceP.y * dim.x / dim.y);
	}


	Point Oval::GetLocalClosestTo(const Point& p)
	{
		Point closestUnit = p.Normalize();
		return Point(closestUnit.x * dim.x, closestUnit.y * dim.y);
	}


	Shape::Intersection Oval::GetIntersectionWith(Shape* other)
	{
		if (other->GetPriority() > GetPriority()) {
			Intersection inter = other->GetIntersectionWith(this);
			inter.normal *= -1;
			return inter;
		}

		if (IsCircle()) {
			return GetIntersectionAsCircleWith(other);
		}

		return Shape::GetIntersectionWith(other);
	}


	void Oval::CheckCorners()
	{
		if (needsToRecalc) {
			Point smallHeight = yAxis * realDim.y * 0.33f;
			Point smallWidth = xAxis * realDim.x * 0.33f;
			Point height = yAxis * realDim.y;
			Point width = xAxis * realDim.x;
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


	void Oval::UpdateFurthest()
	{
		furthest = glm::max(dim.x, dim.y);
	}


	Shape::Intersection Oval::GetIntersectionAsCircleWith(Shape* other)
	{
		if (other->GetType() == Type::OVAL) {
			Oval* otherOval = (Oval*)other;
			if (otherOval->IsCircle()) {
				return GetIntersectionAsCircleWithCircle(otherOval);
			}
		}

		Point center = other->GetRelativePos(pos);
		Point closest = other->GetLocalClosestTo(center);
		return GetIntersectionAsCircleFromClosest(other, closest, center);
	}


	Shape::Intersection Oval::GetIntersectionAsCircleWithCircle(Oval* other)
	{
		float rSum = dim.x + other->dim.x;
		float dist2 = pos.DistanceSquared(other->pos);
		if (dist2 > rSum * rSum) {
			return Intersection();
		}

		Intersection inter;
		inter.depth = (rSum - sqrt(dist2)) / 2;
		inter.normal = (other->pos - pos).Normalize();
		inter.contact = inter.normal * (dim.x - inter.depth);
		return inter;
	}


	Shape::Intersection Oval::GetIntersectionAsCircleFromClosest(Shape* other, const Point& closest, const Point& center)
	{
		if (closest.DistanceSquared(center) > dim.x * dim.x) {
			return Intersection();
		}

		Point localNormal = closest - center;
		Intersection inter;
		inter.normal = (localNormal).Rotate(other->GetRot()).Normalize();
		inter.depth = (dim.x - closest.Distance(center)) / 2;
		inter.contact = pos + inter.normal * (dim.x - inter.depth);
		return inter;
	}

}
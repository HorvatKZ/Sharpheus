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
		Point unscaledP(relP.x * relP.x / dim.x * dim.x, relP.y * relP.y / dim.y * dim.y);
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


	Shape::Intersection Oval::GetIntersectionWith(Oval* other)
	{
		if (other->IsCircle()) {
			Intersection inter = other->GetIntersectionAsCircleWith(this);
			inter.normal *= -1;
			return inter;
		}

		return Intersection();
	}


	Shape::Intersection Oval::GetIntersectionWith(Rect* other)
	{
		return Intersection();
	}


	Shape::Intersection Oval::GetIntersectionWith(Capsule* other)
	{
		return Intersection();
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

		Point localNormal = -1 * other->GetLocalPerpendicularAt(closest);
		Intersection inter;
		inter.normal = (localNormal).Rotate(other->GetRot()).Normalize();
		inter.depth = (dim.x - closest.Distance(center)) / glm::cos(glm::radians((closest - center).GetAngleWith(localNormal))) / 2;
		inter.contact = (closest + localNormal * inter.depth).Rotate(other->GetRot()) + other->GetPos();
		return inter;
	}

}
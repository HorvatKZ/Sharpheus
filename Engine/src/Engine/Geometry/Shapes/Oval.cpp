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


	Shape::Intersection Oval::GetIntersectionAsCircleWith(Oval* other)
	{
		if (other->IsCircle()) {
			return GetIntersectionAsCircleWithCircle(other);
		}

		Point center = other->GetRelativePos(pos);
		Point closest = center.Normalize();
		closest = Point(closest.x * other->dim.x, closest.y * other->dim.y);
		return GetIntersectionAsCircleFromClosest(other, closest, center);
	}


	Shape::Intersection Oval::GetIntersectionAsCircleWith(Rect* other)
	{
		Point center = other->GetRelativePos(pos);
		Point closest;
		Point otherDim = other->GetDim();

		closest.x = glm::clamp(center.x, -otherDim.x, otherDim.x);
		closest.y = glm::clamp(center.y, -otherDim.y, otherDim.y);
		if (closest == center) {
			return GetIntersectionAsInside(other, center);
		}

		return GetIntersectionAsCircleFromClosest(other, closest, center);
	}


	Shape::Intersection Oval::GetIntersectionAsCircleWith(Capsule* other)
	{
		return Intersection();
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


	Shape::Intersection Oval::GetIntersectionAsInside(Oval* other, const Point& center)
	{
		Point otherDim = other->GetDim();
		Point closest = center.Normalize();
		closest = Point(closest.x * other->dim.x, closest.y * other->dim.y);
		return GetIntersectionAsCircleFromClosest(other, closest, center);
	}


	Shape::Intersection Oval::GetIntersectionAsInside(Rect* other, const Point& center)
	{
		Point otherDim = other->GetDim();
		uint8_t ind = 0;
		float dist = center.x + otherDim.x;

		if (center.x - otherDim.x < dist) {
			dist = center.x - otherDim.x;
			ind = 1;
		}
		if (center.y + otherDim.y < dist) {
			dist = center.y + otherDim.y;
			ind = 2;
		}
		if (center.y - otherDim.y < dist) {
			dist = center.y - otherDim.y;
			ind = 3;
		}

		Intersection inter;
		inter.contact = center;
		inter.depth = (dist + dim.x) / 2;
		switch (ind) {
		case 0: // left
			inter.normal = other->GetXAxis() * -1;
			break;
		case 1: // right
			inter.normal = other->GetXAxis();
			break;
		case 2: // top
			inter.normal = other->GetYAxis() * -1;
			break;
		case 3: // bottom
			inter.normal = other->GetYAxis();
			break;
		}
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
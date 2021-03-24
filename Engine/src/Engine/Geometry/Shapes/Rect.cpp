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


	Point Rect::GetLocalClosestTo(const Point& p)
	{
		Point closest;
		closest.x = glm::clamp(p.x, -dim.x, dim.x);
		closest.y = glm::clamp(p.y, -dim.y, dim.y);

		if (closest != p) {
			return closest;
		}


		float dist = p.x + dim.x;
		closest = Point(-dim.x, p.y);

		if (p.x - dim.x < dist) {
			dist = p.x - dim.x;
			closest = Point(dim.x, p.y);
		}
		if (p.y + dim.y < dist) {
			dist = p.y + dim.y;
			closest = Point(p.x, -dim.y);
		}
		if (p.y - dim.y < dist) {
			dist = p.y - dim.y;
			closest = Point(p.x, dim.y);
		}

		return closest;
	}


	Shape::Intersection Rect::GetIntersectionWith(Oval* other)
	{
		Intersection inter = other->GetIntersectionWith((Shape*)this);
		inter.normal *= -1;
		return inter;
	}


	Shape::Intersection Rect::GetIntersectionWith(Rect* other)
	{
		IntersectionData thisData = GetIntesectionDataWith(other);
		IntersectionData otherData = other->GetIntesectionDataWith(this);
		IntersectionData usedData;
		bool otherIsUsed = false;

		if (thisData.smallestContact == -1.f) {
			if (otherData.smallestContact == -1.f) {
				Point* otherC = other->GetCorners();
				for (uint8_t i = 0; i < 4; ++i) {
					if (IsInside(otherC[i])) {
						int i = 5;
					}
				}
				for (uint8_t i = 0; i < 4; ++i) {
					if (other->IsInside(corners[i])) {
						int i = 5;
					}
				}
				return Intersection();
			}
			else {
				usedData = otherData;
				otherIsUsed = true;
			}
		}
		else {
			if (otherData.smallestContact == -1.f) {
				usedData = thisData;
			}
			else {
				otherIsUsed = thisData.smallestContact > otherData.smallestContact;
				usedData = otherIsUsed ? otherData : thisData;
			}
		}

		Intersection inter;
		inter.normal = usedData.normal;
		if (otherIsUsed) {
			inter.normal *= -1;
		}
		inter.depth = usedData.smallestContact / 2;
		Point mostInside = usedData.use2 ? (usedData.contactPoints[0] + usedData.contactPoints[1]) / 2 : usedData.contactPoints[0];
		inter.contact = mostInside + usedData.normal * inter.depth;
		return inter;
	}


	Shape::Intersection Rect::GetIntersectionWith(Capsule* other)
	{
		return Intersection();
	}


	Rect::IntersectionData Rect::GetIntesectionDataWith(Rect* other)
	{
		IntersectionData data;
		Point* otherCorners = other->GetCorners();
		for (uint8_t i = 0; i < 4; ++i) {
			Point relP = GetRelativePos(otherCorners[i]);
			if (-dim.x <= relP.x && relP.x < dim.x && -dim.y <= relP.y && relP.y < dim.y) {
				UpdateSmallestContact(dim.x - relP.x, otherCorners[i], xAxis, data);
				UpdateSmallestContact(dim.x + relP.x, otherCorners[i], xAxis * -1, data);
				UpdateSmallestContact(dim.y - relP.y, otherCorners[i], yAxis, data);
				UpdateSmallestContact(dim.y + relP.y, otherCorners[i], yAxis * -1, data);
			}
		}

		return data;
	}


	void Rect::UpdateSmallestContact(float diff, const Point& newP, const Point& newNormal, IntersectionData& data)
	{
		float tolerance = 0.01f;
		if (data.smallestContact == -1.f || diff < data.smallestContact * (1.f - tolerance)) {
			data.smallestContact = diff;
			data.contactPoints[0] = newP;
			data.use2 = false;
			data.normal = newNormal;
		}
		else if (data.smallestContact * (1.f - tolerance) <= diff && diff <= data.smallestContact * (1.f + tolerance)) {
			data.contactPoints[1] = newP;
			data.use2 = true;
		}
	}

}
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

	
	void Rect::UpdateFurthest()
	{
		furthest = sqrt(dim.x * dim.x + dim.y * dim.y);
	}


	Shape::IntersectionData Rect::GetOneWayIntersectionDataWithSAT(Shape* other)
	{
		if (other->GetType() != Type::RECT) {
			return Shape::GetOneWayIntersectionDataWithSAT(other);
		}

		Point* otherCorners = other->GetCorners();
		Point relCorners[4];
		for (uint8 i = 0; i < 4; ++i) {
			relCorners[i] = GetRelativePos(otherCorners[i]);
		}

		float minX = relCorners[0].x, minY = relCorners[0].y;
		float maxX = minX, maxY = minY;
		for (uint8 i = 1; i < 4; ++i) {
			if (relCorners[i].x < minX) {
				minX = relCorners[i].x;
			}
			if (relCorners[i].x > maxX) {
				maxX = relCorners[i].x;
			}
			if (relCorners[i].y < minY) {
				minY = relCorners[i].y;
			}
			if (relCorners[i].y > maxY) {
				maxY = relCorners[i].y;
			}
		}

		if (maxX < -dim.x || dim.x < minX || maxY < -dim.y || dim.y < minY) {
			return IntersectionData();
		}

		float relevantCoord = maxX;
		bool isXCoord = true;
		IntersectionData data;
		data.smallestContact = dim.x + maxX;
		data.normal = xAxis * -1;
		if (dim.x - minX < data.smallestContact) {
			data.smallestContact = dim.x - minX;
			relevantCoord = minX;
			data.normal = xAxis;
		}

		if (dim.y + maxY < data.smallestContact) {
			data.smallestContact = dim.y + maxY;
			relevantCoord = maxY;
			data.normal = yAxis * -1;
			isXCoord = false;
		}

		if (dim.y - minY < data.smallestContact) {
			data.smallestContact = dim.y - minY;
			relevantCoord = minY;
			data.normal = yAxis;
			isXCoord = false;
		}

		float tolerance = 0.01f;
		uint8 found = 0;
		for (uint8 i = 0; i < 4; ++i) {
			if (isXCoord && abs(relCorners[i].x - relevantCoord) < tolerance ||
				!isXCoord && abs(relCorners[i].y - relevantCoord) < tolerance) {
				data.contactPoints[found++] = otherCorners[i];
			}
		}
		data.use2 = found == 2;
		return data;
	}
}
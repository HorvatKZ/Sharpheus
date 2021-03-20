#include "pch.h"
#include "Point.hpp"


namespace Sharpheus {

	Point Point::Zero = Point(0, 0);
	Point Point::Up = Point(0, -1);
	Point Point::Down = Point(0, 1);
	Point Point::Left = Point(-1, 0);
	Point Point::Right = Point(1, 0);


	Point::Point(float x, float y) : x(x), y(y) {}

	Point::Point(const glm::vec2& vec) : x(vec.x), y(vec.y) {}


	Point Point::operator+(const Point& other) const
	{
		return Point(x + other.x, y + other.y);
	}


	Point Point::operator-(const Point& other) const
	{
		return Point(x - other.x, y - other.y);
	}


	float Point::operator*(const Point& other) const
	{
		return x * other.x + y * other.y;
	}


	Point Point::operator*(float other) const
	{
		return Point(other * x, other * y);
	}

	Point Point::operator/(float other) const
	{
		return Point(x / other, y / other);
	}


	Point& Point::operator+=(const Point& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}


	Point& Point::operator-=(const Point& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}


	Point& Point::operator*=(float other)
	{
		x *= other;
		y *= other;
		return *this;
	}


	Point& Point::operator/=(float other)
	{
		x /= other;
		y /= other;
		return *this;
	}


	bool Point::operator==(const Point& other) const
	{
		return x == other.x && y == other.y;
	}


	bool Point::operator!=(const Point& other) const
	{
		return x != other.x || y != other.y;
	}


	float Point::Length() const
	{
		return sqrt(LengthSquared());
	}


	float Point::LengthSquared() const
	{
		return x * x + y * y;
	}


	float Point::Distance(const Point& other) const
	{
		return sqrt(DistanceSquared(other));
	}


	float Point::DistanceSquared(const Point& other) const
	{
		return (x - other.x) * (x - other.x) + (y - other.y) * (y - other.y);
	}


	Point Point::Normalize() const
	{
		float len = Length();
		return Point(x / len, y / len);
	}


	glm::vec2 Point::ToVec2() const
	{
		return glm::vec2(x, y);
	}


	Point Point::Rotate(float angle) const
	{
		float angleInRad = glm::radians(-angle); // Because y axis is inverted
		float sinus = glm::sin(angleInRad);
		float cosinus = glm::cos(angleInRad);
		return Point(x * cosinus - y * sinus, x * sinus + y * cosinus);
	}


	float Point::GetAngle(const Point& other) const
	{
		return glm::degrees(glm::acos((*this * other) / (this->Length() * other.Length())));
	}


	Point Point::GetUnit(float angle)
	{
		float angleInRad = glm::radians(-angle); // Because y axis is inverted
		return Point(glm::cos(angleInRad), glm::sin(angleInRad));
	}


	bool Point::DoSectionsIntersect(const Point& sec1begin, const Point& sec1end, const Point& sec2begin, const Point& sec2end)
	{
		if (sec1end.x == sec1begin.x && sec2end.x == sec2begin.x) { // if both are vertical
			return false;
		}
		else if (sec1end.x != sec1begin.x && sec2end.x == sec2begin.x) { // if sec1 is non-vertical, and sec2 is vertical
			float sec1slope = (sec1end.y - sec1begin.y) / (sec1end.x - sec1begin.x);
			float sec1offset = sec1begin.y - sec1begin.x * sec1slope;

			float x = sec2begin.x;
			float y = sec2begin.x * sec1slope + sec1offset;

			return (y < sec2begin.y && y > sec2end.y || y > sec2begin.y && y < sec2end.y) &&
				(x < sec1begin.x && x > sec1end.x || x > sec1begin.x && x < sec1end.x);
		}
		else if (sec2end.x != sec2begin.x && sec1end.x == sec1begin.x) { // if sec2 is non-vertical, and sec1 is vertical
			float sec2slope = (sec2end.y - sec2begin.y) / (sec2end.x - sec2begin.x);
			float sec2offset = sec2begin.y - sec2begin.x * sec2slope;

			float x = sec1begin.x;
			float y = sec1begin.x * sec2slope + sec2offset;

			return (y < sec1begin.y&& y > sec1end.y || y > sec1begin.y && y < sec1end.y) &&
				(x < sec2begin.x&& x > sec2end.x || x > sec2begin.x && x < sec2end.x);
		}
		else { // if both are non-vertical
			float sec1slope = (sec1end.y - sec1begin.y) / (sec1end.x - sec1begin.x);
			float sec1offset = sec1begin.y - sec1begin.x * sec1slope;
			float sec2slope = (sec2end.y - sec2begin.y) / (sec2end.x - sec2begin.x);
			float sec2offset = sec2begin.y - sec2begin.x * sec2slope;

			if (sec1slope == sec2slope) { // if they are paralell
				return false;
			}

			float x = (sec2offset - sec1offset) / (sec1slope - sec2slope);

			return (x < sec1begin.x&& x > sec1end.x || x > sec1begin.x && x < sec1end.x) &&
				(x < sec2begin.x&& x > sec2end.x || x > sec2begin.x && x < sec2end.x);
		}
	}

	Point Point::GetNormalVectorToward(const Point& secBegin, const Point& secEnd, const Point& toward)
	{
		if (secBegin.x == secEnd.x) { // vertical
			return (toward.x > secBegin.x) ? Right : Left;
		}
		if (secBegin.y == secEnd.y) { // horizontal
			return (toward.y > secBegin.y) ? Down : Up;
		}

		float secSlope = (secEnd.y - secBegin.y) / (secEnd.x - secBegin.x);
		float secOffset = secBegin.y - secBegin.x * secSlope;
		Point normal = Point(1 / (secBegin.x - secEnd.x), -1 / (secBegin.y - secEnd.y)).Normalize();

		if (toward.x * secSlope + secOffset > toward.y && normal.x * secSlope + secOffset > normal.y ||
			toward.x * secSlope + secOffset < toward.y && normal.x * secSlope + secOffset < normal.y) {
			return normal;
		} else {
			return normal * -1;
		}
	}


	bool Point::IsBelow(const Point& secBegin, const Point& secEnd, const Point& point)
	{
		if (secBegin.x == secEnd.x) { // vertical
			return point.x > secBegin.x;
		}
		if (secBegin.y == secEnd.y) { // horizontal
			return point.y > secBegin.y;
		}

		float secSlope = (secEnd.y - secBegin.y) / (secEnd.x - secBegin.x);
		float secOffset = secBegin.y - secBegin.x * secSlope;
		return secSlope * point.x + secOffset > point.y;
	}


	Point operator*(float lhs, const Point& rhs)
	{
		return Point(lhs * rhs.x, lhs * rhs.y);
	}


	Point operator/(float lhs, const Point& rhs)
	{
		return Point(lhs / rhs.x, lhs / rhs.y);
	}

}
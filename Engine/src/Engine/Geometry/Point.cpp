#include "pch.h"
#include "Point.hpp"


namespace Sharpheus {

	Point Point::Zero = Point(0, 0);
	Point Point::Up = Point(0, -1);
	Point Point::Down = Point(0, 1);
	Point Point::Left = Point(-1, 0);
	Point Point::Right = Point(1, 0);


	Point::Point(float x, float y) : x(x), y(y) {}


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


	Point Point::Rotate(float angle) const
	{
		float angleInRad = glm::radians(-angle); // Because y axis is inverted
		float sinus = glm::sin(angleInRad);
		float cosinus = glm::cos(angleInRad);
		return Point(x * cosinus - y * sinus, x * sinus + y * cosinus);
	}


	float Point::GetAngle() const
	{
		if (x == 0) {
			return (y > 0) ? 270 : (y == 0) ? 0 : 90;
		}
		
		if (y == 0) {
			return (x >= 0) ? 0 : 180;
		}

		float angle = glm::degrees(glm::atan(-y / x));
		if (x < 0 && y < 0) {
			return angle - 180;
		}
		
		if (x < 0) {
			return 180 + angle;
		}

		return angle;
	}


	float Point::GetAngleWith(const Point& other) const
	{
		return glm::degrees(glm::acos((*this * other) / (this->Length() * other.Length())));
	}


	Point Point::GetUnit(float angle)
	{
		float angleInRad = glm::radians(-angle); // Because y axis is inverted
		return Point(glm::cos(angleInRad), glm::sin(angleInRad));
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
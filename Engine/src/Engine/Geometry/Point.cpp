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
		x += other;
		y += other;
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
		return sqrt(x * x + y * y);
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
		return glm::acos((*this * other) / (this->Length() * other.Length()));
	}


	Point operator*(float lhs, const Point& rhs)
	{
		return Point(lhs * rhs.x, lhs * rhs.y);
	}

}
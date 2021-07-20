#pragma once

#include <glm/glm.hpp>


namespace Sharpheus {

	struct SPH_EXPORT Point
	{
		float x, y;

		Point(float x = 0.f, float y = 0.f);

		Point			operator+(const Point& other) const;
		Point			operator-(const Point& other) const;
		float			operator*(const Point& other) const;
		Point			operator*(float other) const;
		friend Point	operator*(float lhs, const Point& rhs);
		Point			operator/(float other) const;
		friend Point	operator/(float lhs, const Point& rhs);

		Point&			operator+=(const Point& other);
		Point&			operator-=(const Point& other);
		Point&			operator*=(float other);
		Point&			operator/=(float other);

		bool			operator==(const Point& other) const;
		bool			operator!=(const Point& other) const;

		float			Length() const;
		float			LengthSquared() const;
		float			Distance(const Point& other) const;
		float			DistanceSquared(const Point& other) const;
		Point			Normalize() const;

		Point			Rotate(float angle) const;
		float			GetAngle() const;
		float			GetAngleWith(const Point& other) const;

		static const Point Zero;
		static const Point Up;
		static const Point Down;
		static const Point Left;
		static const Point Right;

		static Point GetUnit(float angle);
	};

}
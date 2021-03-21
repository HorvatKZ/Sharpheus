#pragma once

#include <glm/glm.hpp>


namespace Sharpheus {

	struct SPH_EXPORT Point
	{
		float x, y;

		Point(float x = 0.f, float y = 0.f);
		Point(const glm::vec2& vec);

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
		glm::vec2		ToVec2() const;

		Point			Rotate(float angle) const;
		float			GetAngle() const;
		float			GetAngleWith(const Point& other) const;

		static Point Zero;
		static Point Up;
		static Point Down;
		static Point Left;
		static Point Right;

		static Point GetUnit(float angle);

		static bool DoSectionsIntersect(const Point& sec1begin, const Point& sec1end, const Point& sec2begin, const Point& sec2end);
		static Point GetNormalVectorToward(const Point& secBegin, const Point& secEnd, const Point& toward);
		static bool IsBelow(const Point& secBegin, const Point& secEnd, const Point& point);
	};

}
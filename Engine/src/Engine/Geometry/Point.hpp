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

		Point&			operator+=(const Point& other);
		Point&			operator-=(const Point& other);
		Point&			operator*=(float other);

		bool			operator==(const Point& other) const;
		bool			operator!=(const Point& other) const;

		float			Length() const;
		glm::vec2		ToVec2() const;

		Point			Rotate(float angle) const;
		float			GetAngle(const Point& other) const;

		static Point Zero;
		static Point Up;
		static Point Down;
		static Point Left;
		static Point Right;
	};

}
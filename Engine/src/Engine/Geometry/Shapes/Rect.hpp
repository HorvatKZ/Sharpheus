#pragma once

#include "Shape.hpp"

namespace Sharpheus {

	class Rect : public Shape
	{
	public:
		Rect() = default;
		Rect(const Point& pos, const Point& dim, float rot);
		virtual ~Rect();

		virtual bool IsInside(const Point& p);

		CollData GetCollWith(const class Oval& other);
		CollData GetCollWith(const class Rect& other);
		CollData GetCollWith(const class Capsule& other);
	};

}
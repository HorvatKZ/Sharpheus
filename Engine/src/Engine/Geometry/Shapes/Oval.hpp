#pragma once

#include "Shape.hpp"


namespace Sharpheus {

	class Oval : public Shape
	{
	public:
		Oval() = default;
		Oval(const Point& pos, const Point& dim, float rot);
		virtual ~Oval();

		virtual bool IsInside(const Point& p);

		CollData GetCollWith(const class Oval& other);
		CollData GetCollWith(const class Rect& other);
		CollData GetCollWith(const class Capsule& other);
	};

}
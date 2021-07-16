#pragma once

#include "Point.hpp"


namespace Sharpheus {

	struct SPH_EXPORT Transform {
		Point pos;
		Point scale;
		float rot;

		Transform(const Point& pos = Point(), const Point& scale = Point(1.f, 1.f), float rot = 0) : pos(pos), scale(scale), rot(rot) {}

		bool operator==(const Transform& other) const {
			return pos == other.pos && scale == other.scale && rot == other.rot;
		}

		bool operator!=(const Transform& other) const {
			return pos != other.pos || scale != other.scale || rot != other.rot;
		}

		friend std::ostream& operator<<(std::ostream& out, const Transform& trafo);

		// Not commutative (a + b != b + a)
		Transform operator+(const Transform& other) const;

		// if c = a + b, then b = c - a, but a != c - b
		Transform SubstractFirst(const Transform& other) const;

		// if c = a + b, then a = c - b, but b != c - a
		Transform SubstractSecond(const Transform& other) const;
	};


	

}
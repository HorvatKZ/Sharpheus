#pragma once

#include "Point.hpp"


namespace Sharpheus {

	struct Transform {
		Point pos;
		Point scale;
		float rot;

		Transform(const Point& pos = Point(), const Point& scale = Point(1.f, 1.f), float rot = 0) : pos(pos), scale(scale), rot(rot) {}

		bool operator==(const Transform& other) {
			return pos == other.pos && scale == other.scale && rot == other.rot;
		}

		bool operator!=(const Transform& other) {
			return pos != other.pos || scale != other.scale || rot != other.rot;
		}

		// Not commutative (a + b != b + a)
		Transform operator+(const Transform& other) const {
			Transform result = *this;
			result.pos += Point(other.pos.x * scale.x, other.pos.y * scale.y).Rotate(rot);
			result.scale.x *= other.scale.x;
			result.scale.y *= other.scale.y;
			result.rot += other.rot;
			return result;
		}

		// if c = a + b, then b = c - a, but a != c - b
		Transform SubstractFirst(const Transform& other) const {
			Transform result = *this;
			result.pos = (pos - other.pos).Rotate(-other.rot);
			result.pos = Point(result.pos.x / other.scale.x, result.pos.y / other.scale.y);
			result.scale.x /= other.scale.x;
			result.scale.y /= other.scale.y;
			result.rot -= other.rot;
			return result;
		}

		// if c = a + b, then a = c - b, but b != c - a
		Transform SubstractSecond(const Transform& other) const {
			Transform result = *this;
			result.scale.x /= other.scale.x;
			result.scale.y /= other.scale.y;
			result.rot -= other.rot;
			result.pos = this->pos - Point(other.pos.x * result.scale.x, other.pos.y * result.scale.y).Rotate(result.rot);
			return result;
		}
	};

}
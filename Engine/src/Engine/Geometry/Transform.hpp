#pragma once

#include "Point.hpp"


namespace Sharpheus {

	struct Transform {
		Point pos = Point(0.f, 0.f);
		Point scale = Point(1.f, 1.f);
		float rot = 0;

		Transform& operator+(const Transform& other) const {
			Transform result = *this;
			result.pos += other.pos.Rotate(other.rot);
			result.scale.x *= other.scale.x;
			result.scale.y *= other.scale.y;
			result.rot += other.rot;
			return result;
		}
	};

}
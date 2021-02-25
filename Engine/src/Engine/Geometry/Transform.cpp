#include "pch.h"
#include "Transform.hpp"


namespace Sharpheus {

	std::ostream& operator<<(std::ostream& out, const Transform& trafo) {
		out << trafo.pos.x << " " << trafo.pos.y << " " << trafo.scale.x << " " << trafo.scale.y << " " << trafo.rot;
		return out;
	}


	Transform Transform::operator+(const Transform& other) const {
		Transform result = *this;
		result.pos += Point(other.pos.x * scale.x, other.pos.y * scale.y).Rotate(rot);
		result.scale.x *= other.scale.x;
		result.scale.y *= other.scale.y;
		result.rot += other.rot;
		return result;
	}


	Transform Transform::SubstractFirst(const Transform& other) const {
		Transform result = *this;
		result.pos = (pos - other.pos).Rotate(-other.rot);
		result.pos = Point(result.pos.x / other.scale.x, result.pos.y / other.scale.y);
		result.scale.x /= other.scale.x;
		result.scale.y /= other.scale.y;
		result.rot -= other.rot;
		return result;
	}


	Transform Transform::SubstractSecond(const Transform& other) const {
		Transform result = *this;
		result.scale.x /= other.scale.x;
		result.scale.y /= other.scale.y;
		result.rot -= other.rot;
		result.pos = this->pos - Point(other.pos.x * result.scale.x, other.pos.y * result.scale.y).Rotate(result.rot);
		return result;
	}
}
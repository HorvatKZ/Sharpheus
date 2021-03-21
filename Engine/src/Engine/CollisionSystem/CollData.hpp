#pragma once

#include "Engine/Geometry/Shapes/Shape.hpp"

namespace Sharpheus {

	struct CollData {
		Shape::Intersection geom;
		bool isDynamic = false;
		Point vOther;
		float mOther = 0.f;

		inline bool Is() { return geom.depth > 0.f; }
	};

	typedef std::pair<CollData, CollData> CollDataPair;
}
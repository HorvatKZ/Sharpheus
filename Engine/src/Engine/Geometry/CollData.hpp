#pragma once

#include "Point.hpp"


namespace Sharpheus {

	struct CollData {
		Point contact, dir;
		float depth = 0.f;
	};

}
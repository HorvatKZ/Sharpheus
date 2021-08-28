#pragma once

#include "typedefs.h"


namespace Sharpheus {

	struct SPH_EXPORT Color
	{
		byte r, g, b, a;

		Color() : r(0), g(0), b(0), a(255) {}
		Color(byte r, byte g, byte b, byte a = 255) : r(r), g(g), b(b), a(a) {}

		inline float GetRed() const { return r / 255.f; }
		inline float GetGreen() const { return g / 255.f; }
		inline float GetBlue() const { return b / 255.f; }
		inline float GetAlpha() const { return a / 255.f; }

		inline void MakeOpaque(float percent) { a *= percent; }

		inline glm::vec3 ToVec3() const { return glm::vec3(GetRed(), GetGreen(), GetBlue()); }
		inline glm::vec4 ToVec4() const { return glm::vec4(GetRed(), GetGreen(), GetBlue(), GetAlpha()); }

		static const Color Black;
		static const Color Red;
		static const Color Green;
		static const Color Blue;
		static const Color Yellow;
		static const Color Cyan;
		static const Color Magenta;
		static const Color White;
		static const Color Transparent;
	};
}
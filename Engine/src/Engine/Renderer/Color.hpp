#pragma once

namespace Sharpheus {

	struct SPH_EXPORT Color
	{
		uint8_t r, g, b, a;

		Color() : r(0), g(0), b(0), a(255) {}
		Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) : r(r), g(g), b(b), a(a) {}

		inline float GetRed() const { return r / 255.f; }
		inline float GetGreen() const { return g / 255.f; }
		inline float GetBlue() const { return b / 255.f; }
		inline float GetAlpha() const { return a / 255.f; }

		static Color Black;
		static Color Red;
		static Color Green;
		static Color Blue;
		static Color Yellow;
		static Color Cyan;
		static Color Magenta;
		static Color White;
		static Color Transparent;
	};
}
#pragma once

#include "Image.hpp"
#include <cstdio>

#define SPH_FONT_BOLD 0x01
#define SPH_FONT_ITALIC 0x02
#define SPH_FONT_UNDERLINED 0x04


namespace Sharpheus {

	class Font : public Resource
	{
	public:
		Font(const std::string& fontFile, Image* img);
		virtual ~Font();

		inline Image* GetImage() { return img; }
		inline const std::string& GetName() { return name; }

		void Render(const std::string& text, const Point& center, const Point& xAxis, const Point& yAxis,
			float size, const Color& color, uint8_t style = 0);

		Point GetExtent(const std::string& text, float size, uint8_t style = 0);

	private:
		struct CharData {
			Point texCoords[4];
			float width, height, xoffset, yoffset, xadvance;
		};

		Image* img;
		std::string name;
		float lineHeight;
		CharData chars[128];

		static float boldness;
		static uint8_t boldIterations;
		static float italicness;
		static float underlineHeight;
		static float underlineThickness;

		void RenderChar(char character, Point& pos, const Point& xAxis, const Point& yAxis,
			float size, const Color& color, uint8_t style = 0);

		void ReadFontData();
		void GetNextKeyValue(FILE* fin, std::string& key, std::string& value);
		void ReadCharData(FILE* fin, float originalSize);
	};

}
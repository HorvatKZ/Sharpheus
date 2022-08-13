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
		Font(const std::string& fontFile, const Image* img);
		virtual ~Font();

		inline const Image* GetImage() const { return img; }
		inline const std::string& GetName() const { return name; }

		void Render(const std::string& text, const Point& center, float size, const Color& color = Color::White,
			const Point& xAxis = Point::Right, const Point& yAxis = Point::Down, byte style = 0) const;

		Point GetExtent(const std::string& text, float size, byte style = 0) const;

	private:
		struct CharData {
			Point texCoords[4];
			float width, height, xoffset, yoffset, xadvance;
		};

		const Image* img;
		std::string name;
		float lineHeight;
		CharData chars[128];

		static const float boldness;
		static const uint8 boldIterations;
		static const float italicness;
		static const float underlineHeight;
		static const float underlineThickness;

		void RenderChar(char character, Point& pos, const Point& xAxis, const Point& yAxis,
			float size, const Color& color, byte style = 0) const;

		void ReadFontData();
		void GetNextKeyValue(FILE* fin, std::string& key, std::string& value);
		void ReadCharData(FILE* fin, float originalSize);
	};

}
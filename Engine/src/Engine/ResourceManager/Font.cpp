#include "pch.h"
#include "Font.hpp"


namespace Sharpheus {

	float Font::boldness = 0.02f;
	uint8_t Font::boldIterations = 3;
	float Font::italicness = 0.15f;
	float Font::underlineHeight = -0.35f;
	float Font::underlineThickness = 0.05f;

	Font::Font(const std::string& fontFile, Image* img)
		: Resource(fontFile), img(img)
	{
		ReadFontData();
	}


	Font::~Font()
	{
	}


	void Font::Render(const std::string& text, const Point& center, const Point& xAxis, const Point& yAxis,
		float size, const Color& color, uint8_t style)
	{
		Point halfExtent = GetExtent(text, size, style) * 0.5f;
		Point renderPos = center - xAxis * halfExtent.x - yAxis * halfExtent.y;
		for (uint32_t i = 0; i < text.length(); ++i) {
			RenderChar(text[i], renderPos, xAxis, yAxis, size, color, style);
		}

		if (style & SPH_FONT_UNDERLINED) {
			Point coords[4] = {
				center - xAxis * halfExtent.x - yAxis * lineHeight * underlineHeight * size,
				center + xAxis * halfExtent.x - yAxis * lineHeight * underlineHeight * size,
				center + xAxis * halfExtent.x + yAxis * lineHeight * (underlineThickness - underlineHeight) * size,
				center - xAxis * halfExtent.x + yAxis * lineHeight * (underlineThickness - underlineHeight) * size
			};
			Renderer::DrawMonocromeQuad(coords, color);
		}
	}


	Point Font::GetExtent(const std::string& text, float size, uint8_t style)
	{
		float width = 0.f;
		for (uint32_t i = 0; i < text.length(); ++i) {
			width += size * chars[text[i]].xadvance;
		}
		if (style & SPH_FONT_BOLD) {
			width += lineHeight * boldness * size;
		}
		if (style & SPH_FONT_ITALIC) {
			width += lineHeight * italicness * size;
		}

		return Point(width, lineHeight * size);
	}


	void Font::RenderChar(char character, Point& pos, const Point& xAxis, const Point& yAxis,
		float size, const Color& color, uint8_t style)
	{
		CharData data = chars[character];

		float italicShift = (style & SPH_FONT_ITALIC) ? lineHeight * italicness : 0.f;
		Point start = pos + xAxis * data.xoffset * size + yAxis * data.yoffset * size;
		Point coords[4] = {
			start + xAxis * italicShift * size,
			start + xAxis * (data.width + italicShift) * size,
			start + xAxis * data.width * size + yAxis * data.height * size,
			start + yAxis * data.height * size
		};
		img->RenderPart(coords, data.texCoords, color);

		if (style & SPH_FONT_BOLD) {
			float boldShift = lineHeight * boldness * size / boldIterations;
			for (uint8_t i = 0; i < boldIterations; ++i) {
				for (uint8_t j = 0; j < 4; ++j) {
					coords[j] += xAxis * boldShift;
				}
				img->RenderPart(coords, data.texCoords, color);
			}
		}

		pos += xAxis * data.xadvance * size;
	}


	void Font::ReadFontData()
	{
		SPH_LOG("Importing font: \"{0}\"", fullPath);
		FILE* fin;
		fin = fopen(fullPath.c_str(), "r");
		SPH_ASSERT(fin != NULL, "Unable to open font file: \"{0}\"", fullPath);

		std::string key, value;
		uint32_t basicSize;
		while (key != "count") {
			GetNextKeyValue(fin, key, value);
			
			if (key == "face") {
				name = value;
			}
			else if (key == "size") {
				basicSize = std::stoi(value);
			}
			else if (key == "lineHeight") {
				lineHeight = std::stoi(value);
			}
		}
		lineHeight /= basicSize;

		uint8_t n = std::stoi(value);
		for (uint8_t i = 0; i < n; ++i) {
			ReadCharData(fin, basicSize);
		}

		fclose(fin);
	}


	void Font::GetNextKeyValue(FILE* fin, std::string& key, std::string& value)
	{
		key.clear();
		value.clear();

		char c = fgetc(fin);
		while (c != EOF && c != '=') {
			if (c == ' ' || c == '\n') {
				key.clear();
			} else {
				key += c;
			}
			c = fgetc(fin);
		}
		if (c == EOF) {
			key.clear();
			value.clear();
			return;
		}

		c = fgetc(fin);
		bool isMultiWord = c == '\"';
		if (!isMultiWord) {
			value += c;
		}
		c = fgetc(fin);
		while (c != EOF && c != '\n' && ((!isMultiWord && c != ' ') || (isMultiWord && c !='\"'))) {
			value += c;
			c = fgetc(fin);
		}

		if (c != EOF && isMultiWord) {
			c = fgetc(fin);
		}
	}


	void Font::ReadCharData(FILE* fin, float originalSize)
	{
		std::string key, value;
		GetNextKeyValue(fin, key, value);
		SPH_ASSERT(key == "id", "Not appropriate format");
		uint8_t id = std::stoi(value);
		CharData& data = chars[id];

		GetNextKeyValue(fin, key, value);
		SPH_ASSERT(key == "x", "Not appropriate format");
		uint32_t x = std::stoi(value);

		GetNextKeyValue(fin, key, value);
		SPH_ASSERT(key == "y", "Not appropriate format");
		uint32_t y = std::stoi(value);

		GetNextKeyValue(fin, key, value);
		SPH_ASSERT(key == "width", "Not appropriate format");
		uint32_t width = std::stoi(value);

		GetNextKeyValue(fin, key, value);
		SPH_ASSERT(key == "height", "Not appropriate format");
		uint32_t height = std::stoi(value);

		GetNextKeyValue(fin, key, value);
		SPH_ASSERT(key == "xoffset", "Not appropriate format");
		data.xoffset = std::stoi(value) / originalSize;

		GetNextKeyValue(fin, key, value);
		SPH_ASSERT(key == "yoffset", "Not appropriate format");
		data.yoffset = std::stoi(value) / originalSize;

		GetNextKeyValue(fin, key, value);
		SPH_ASSERT(key == "xadvance", "Not appropriate format");
		data.xadvance = std::stoi(value) / originalSize;

		data.width = width / originalSize;
		data.height = height / originalSize;
		float imgWidth = img->GetWidth(), imgHeight = img->GetHeight();
		data.texCoords[0] = Point(x / imgWidth, y / imgHeight);
		data.texCoords[1] = Point((x + width) / imgWidth, y / imgHeight);
		data.texCoords[2] = Point((x + width) / imgWidth, (y + height) / imgHeight);
		data.texCoords[3] = Point(x / imgWidth, (y + height) / imgHeight);

		char c = '_';
		while (c != EOF && c != '\n') {
			c = fgetc(fin);
		}
	}

}
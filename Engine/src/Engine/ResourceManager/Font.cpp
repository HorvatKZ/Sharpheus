#include "pch.h"
#include "Font.hpp"
#include "Engine/Renderer/Renderer.hpp"


namespace Sharpheus {

	const float Font::boldness = 0.02f;
	const uint8 Font::boldIterations = 3;
	const float Font::italicness = 0.15f;
	const float Font::underlineHeight = -0.35f;
	const float Font::underlineThickness = 0.05f;

	Font::Font(const std::string& fontFile, const Image* img)
		: Resource(fontFile), img(img)
	{
		ReadFontData();
	}


	Font::~Font()
	{
	}


	void Font::Render(const std::string& text, const Point& center, float size, const Color& color,
		const Point& xAxis, const Point& yAxis, byte style) const
	{
		Point halfExtent = GetExtent(text, size, style) * 0.5f;
		Point renderPos = center - xAxis * halfExtent.x - yAxis * halfExtent.y;
		for (uint32 i = 0; i < text.length(); ++i) {
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


	Point Font::GetExtent(const std::string& text, float size, byte style) const
	{
		float width = 0.f;
		for (uint32 i = 0; i < text.length(); ++i) {
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
		float size, const Color& color, byte style) const
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
		img->RenderPart(coords, data.texCoords, false, color);

		if (style & SPH_FONT_BOLD) {
			float boldShift = lineHeight * boldness * size / boldIterations;
			for (uint8 i = 0; i < boldIterations; ++i) {
				for (uint8 j = 0; j < 4; ++j) {
					coords[j] += xAxis * boldShift;
				}
				img->RenderPart(coords, data.texCoords, false, color);
			}
		}

		pos += xAxis * data.xadvance * size;
	}


	void Font::ReadFontData()
	{
		SPH_LOG("Importing font: \"{0}\"", fullPath);
		FILE* fin;
		fin = fopen(fullPath.c_str(), "r");
		if (fin == NULL) {
			SPH_ERROR("Unable to open font file: \"{0}\"", fullPath);
			return;
		}

		std::string key, value;
		uint32 basicSize;
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

		valid = img->IsValid();
		uint8 n = std::stoi(value);
		for (uint8 i = 0; i < n; ++i) {
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
		if (key != "id") {
			SPH_ERROR("Font: Not appropriate format. Cannot find \"id\"");
			valid = false;
		}
		uint8 id = std::stoi(value);
		CharData& data = chars[id];

		GetNextKeyValue(fin, key, value);
		if (key != "x") {
			SPH_ERROR("Font: Not appropriate format. Cannot find \"x\"");
			valid = false;
		}
		uint32 x = std::stoi(value);

		GetNextKeyValue(fin, key, value);
		if (key != "y") {
			SPH_ERROR("Font: Not appropriate format. Cannot find \"y\"");
			valid = false;
		}
		uint32 y = std::stoi(value);

		GetNextKeyValue(fin, key, value);
		if (key != "width") {
			SPH_ERROR("Font: Not appropriate format. Cannot find \"width\"");
			valid = false;
		}
		uint32 width = std::stoi(value);

		GetNextKeyValue(fin, key, value);
		if (key != "height") {
			SPH_ERROR("Font: Not appropriate format. Cannot find \"height\"");
			valid = false;
		}
		uint32 height = std::stoi(value);

		GetNextKeyValue(fin, key, value);
		if (key != "xoffset") {
			SPH_ERROR("Font: Not appropriate format. Cannot find \"xoffset\"");
			valid = false;
		}
		data.xoffset = std::stoi(value) / originalSize;

		GetNextKeyValue(fin, key, value);
		if (key != "yoffset") {
			SPH_ERROR("Font: Not appropriate format. Cannot find \"yoffset\"");
			valid = false;
		}
		data.yoffset = std::stoi(value) / originalSize;

		GetNextKeyValue(fin, key, value);
		if (key != "xadvance") {
			SPH_ERROR("Font: Not appropriate format. Cannot find \"xadvance\"");
			valid = false;
		}
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
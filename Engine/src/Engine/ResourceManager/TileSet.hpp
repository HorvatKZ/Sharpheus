#pragma once

#include "Image.hpp"

namespace Sharpheus {

	class SPH_EXPORT TileSet : public Resource
	{
	public:
		TileSet(const std::string& tileSetFile);
		TileSet(Image* atlas, uint32_t frameWidth, uint32_t frameHeight);
		virtual ~TileSet();

		inline const std::string& GetName() { return name; }
		inline Image* GetAtlas() { return atlas; }
		inline uint32_t GetFrameWidth() { return frameWidth; }
		inline uint32_t GetFrameHeight() { return frameHeight; }
		inline uint32_t GetFrameRows() { return frameRows; }
		inline uint32_t GetFrameCols() { return frameCols; }
		inline uint8_t GetNumOfTiles() { 
			uint32_t tiles = frameCols * frameRows;
			return (tiles > 255) ? 255 : tiles;
		}

		inline void SetName(const std::string& name) { this->name = name; }

		void Render(Point coords[4], uint8_t ind, const Color& tint);

		bool Save();
		bool Save(const std::string& path);

	private:
		std::string name;
		Image* atlas;
		uint32_t frameWidth, frameHeight, frameCols, frameRows;
		Point* texCoords = nullptr;

		bool Load();
		void CalcTexCoords();
	};

}
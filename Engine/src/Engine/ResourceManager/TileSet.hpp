#pragma once

#include "Image.hpp"

namespace Sharpheus {

	class SPH_EXPORT TileSet : public Resource
	{
	public:
		TileSet(const std::string& tileSetFile);
		TileSet(Image* atlas, uint32 frameWidth, uint32 frameHeight);
		virtual ~TileSet();

		inline const std::string& GetName() { return name; }
		inline Image* GetAtlas() { return atlas; }
		inline uint32 GetFrameWidth() { return frameWidth; }
		inline uint32 GetFrameHeight() { return frameHeight; }
		inline uint32 GetFrameRows() { return frameRows; }
		inline uint32 GetFrameCols() { return frameCols; }
		inline uint8 GetNumOfTiles() { 
			uint32 tiles = frameCols * frameRows;
			return (tiles > 255) ? 255 : tiles;
		}

		inline void SetName(const std::string& name) { this->name = name; }

		void Render(Point coords[4], byte ind, const Color& tint);

		bool Save();
		bool Save(const std::string& path);

	private:
		std::string name;
		Image* atlas;
		uint32 frameWidth, frameHeight, frameCols, frameRows;
		Point* texCoords = nullptr;

		bool Load();
		void CalcTexCoords();
	};

}
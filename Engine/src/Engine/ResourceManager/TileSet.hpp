#pragma once

#include "Image.hpp"

namespace Sharpheus {

	class SPH_EXPORT TileSet : public Resource
	{
	public:
		TileSet(const std::string& tileSetFile);
		TileSet(const Image* atlas, uint32 frameWidth, uint32 frameHeight);
		virtual ~TileSet();

		inline const std::string& GetName() const { return name; }
		inline const Image* GetAtlas() const { return atlas; }
		inline uint32 GetFrameWidth() const { return frameWidth; }
		inline uint32 GetFrameHeight() const { return frameHeight; }
		inline uint32 GetFrameRows() const { return frameRows; }
		inline uint32 GetFrameCols() const { return frameCols; }
		inline uint8 GetNumOfTiles() const {
			uint32 tiles = frameCols * frameRows;
			return (tiles > 255) ? 255 : tiles;
		}

		inline void SetName(const std::string& name) { this->name = name; }

		void Render(const Point coords[4], byte ind, const Color& tint = Color::White) const;

		bool Save();
		bool Save(const std::string& path);

	private:
		std::string name;
		const Image* atlas;
		uint32 frameWidth, frameHeight, frameCols, frameRows;
		Point* texCoords = nullptr;

		bool Load();
		void CalcTexCoords();
	};

}
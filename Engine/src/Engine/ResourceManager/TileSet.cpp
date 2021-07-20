#include "pch.h"
#include "TileSet.hpp"
#include "Engine/FileUtils/FileLoader.hpp"
#include "Engine/FileUtils/FileSaver.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"


namespace Sharpheus {

	TileSet::TileSet(const std::string& tileSetFile)
		: Resource(tileSetFile)
	{
		valid = Load();
	}


	TileSet::TileSet(Image* atlas, uint32 frameWidth, uint32 frameHeight)
		: Resource(), atlas(atlas), frameWidth(frameWidth), frameHeight(frameHeight)
	{
		if (atlas != nullptr) {
			frameCols = atlas->GetWidth() / frameWidth;
			frameRows = atlas->GetHeight() / frameHeight;
			CalcTexCoords();
			valid = atlas->IsValid();
		}
		else {
			SPH_ERROR("The texture atlas must be set");
		}
	}


	TileSet::~TileSet()
	{
		delete[] texCoords;
	}


	void TileSet::Render(Point coords[4], byte ind, const Color& tint)
	{
		atlas->RenderPart(coords, texCoords + 4 * ind, tint);
	}


	bool TileSet::Save()
	{
		SPH_ASSERT(HasPath(), "No path set to TileSet");
		return Save(fullPath);
	}


	bool TileSet::Save(const std::string& path)
	{
		SetPath(path);

		FileSaver fs(fullPath);
		SPH_ASSERT(fs.GetStatus(), "Cannot open the location: {0}", fullPath);

		fs.Write(name);
		fs.Write(atlas);
		fs.Write(frameWidth);
		fs.Write(frameHeight);
		return fs.GetStatus();
	}


	bool TileSet::Load()
	{
		FileLoader fl(fullPath);
		SPH_ASSERT(fl.GetStatus(), "Cannot open the location: {0}", fullPath);

		fl.Read(name);
		fl.Read(&atlas);
		fl.Read(frameWidth);
		fl.Read(frameHeight);

		if (atlas == nullptr) {
			SPH_ERROR("The TileSet atlas from {0} is not valid!", fullPath);
			return false;
		}

		frameCols = atlas->GetWidth() / frameWidth;
		frameRows = atlas->GetHeight() / frameHeight;
		CalcTexCoords();
		return fl.GetStatus() && atlas->IsValid();
	}


	void TileSet::CalcTexCoords()
	{
		uint32 tiles = GetNumOfTiles();
		texCoords = new Point[tiles * 4];
		for (uint32 tile = 0; tile < tiles; ++tile) {
			float xStart = ((tile % frameCols) * frameWidth) / (float)atlas->GetWidth();
			float yStart = ((tile / frameCols) * frameHeight) / (float)atlas->GetHeight();
			float xEnd = xStart + frameWidth / (float)atlas->GetWidth();
			float yEnd = yStart + frameHeight / (float)atlas->GetHeight();
			texCoords[tile * 4] = Point(xStart, yStart);
			texCoords[tile * 4 + 1] = Point(xEnd, yStart);
			texCoords[tile * 4 + 2] = Point(xEnd, yEnd);
			texCoords[tile * 4 + 3] = Point(xStart, yEnd);
		}
	}
}
#include "pch.h"
#include "TileMap.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"


namespace Sharpheus {

	ClassInfo TileMap::classInfo("TileMap", "tilemap.png", {
		new TileSetProvider<TileMap>("TileSet", SPH_BIND_GETTER(TileMap::GetTileSet), SPH_BIND_SETTER(TileMap::SetTileSet), SPH_BIND_2(TileMap::SetTileSetFromPath)),
		new ColorProvider<TileMap>("Tint", SPH_BIND_GETTER(TileMap::GetTint), SPH_BIND_SETTER(TileMap::SetTint)),
		new TileMapProvider("TileMap Editor")
	});

	const int32_t TileMap::chunkSize = 16;


	TileMap::TileMap(GameObject* parent, const std::string& name) :
		AxisShapedGameObject(parent, name, new Rect()) {
		prevAxis[0] = Point::Right;
		prevAxis[1] = Point::Up;
		Set({ 0,0 }, 1);
		Set({ 1,0 }, 1);
		Set({ 1,1 }, 2);
	}


	void TileMap::CopyFrom(GameObject* other)
	{
		SPH_CHECKTYPE(other, TileMap);

		AxisShapedGameObject::CopyFrom(other);
		TileMap* trueOther = (TileMap*)other;
		SetTileSet(trueOther->tiles);
		tint = trueOther->tint;
	}


	void TileMap::SetTileSetFromPath(const std::string& path)
	{
		SetTileSet(ResourceManager::GetTileSet(path));
	}


	uint8_t TileMap::Get(const IntPoint& pos)
	{
		IntPoint chunk = pos.ToChunk();
		auto it = chunks.find(chunk);
		if (it == chunks.end()) {
			return 0;
		}

		return it->second.arr[pos.ToRelInd()];
	}

	void TileMap::Set(const IntPoint& pos, uint8_t value)
	{
		if (value == 0) {
			Clear(pos);
			return;
		}

		IntPoint chunk = pos.ToChunk();
		auto it = chunks.find(chunk);
		if (it == chunks.end()) {
			it = chunks.insert({ chunk, ChunkData() }).first;
			memset(it->second.arr, 0, chunkSize * chunkSize);
		}
		uint8_t relInd = pos.ToRelInd();
		uint8_t oldValue = it->second.arr[relInd];
		if (oldValue == 0) {
			++(it->second.count);
		}
		it->second.arr[relInd] = value;
	}


	void TileMap::Clear(const IntPoint& pos)
	{
		IntPoint chunk = pos.ToChunk();
		auto it = chunks.find(chunk);
		if (it == chunks.end()) {
			return;
		}

		uint8_t relInd = pos.ToRelInd();
		uint8_t oldValue = it->second.arr[relInd];
		if (oldValue == 0) {
			return;
		}

		it->second.arr[relInd] = 0;
		--(it->second.count);

		if (it->second.count == 0) {
			chunks.erase(it);
		}
	}


	void TileMap::Render()
	{
		if (tiles != nullptr) {
			if (xAxis != prevAxis[0] || yAxis != prevAxis[1]) {
				UpdateTileCorners();
				prevAxis[0] = xAxis;
				prevAxis[1] = yAxis;
			}

			for (auto it = chunks.begin(); it != chunks.end(); ++it) {
				uint8_t* arr = it->second.arr;
				for (uint32_t i = 0; i < chunkSize * chunkSize; ++i) {
					if (arr[i] != 0) {
						tiles->Render(GetCornersOf(it->first, i), arr[i] - 1, tint);
					}
				}
			}
		}
	}


	void TileMap::RenderSelection()
	{
		if (tiles != nullptr) {
			for (auto it = chunks.begin(); it != chunks.end(); ++it) {
				uint8_t* arr = it->second.arr;
				for (uint32_t i = 0; i < chunkSize * chunkSize; ++i) {
					if (arr[i] != 0) {
						Renderer::DrawMonocromeQuad(GetCornersOf(it->first, i), selectColor);
					}
				}
			}
		}
	}


	bool TileMap::IsSelected(const Point& pos)
	{
		return tiles != nullptr && Get(ToTile(pos)) != 0;
	}


	void TileMap::SetBack(uint8_t max)
	{
		for (auto it = chunks.begin(); it != chunks.end(); ++it) {
			uint8_t* arr = it->second.arr;
			for (uint32_t i = 0; i < chunkSize * chunkSize; ++i) {
				if (arr[i] > max) {
					arr[i] = 0;
					--(it->second.count);
				}
			}
		}
	}


	void TileMap::UpdateTileCorners()
	{
		if (tiles != nullptr) {
			xShift = xAxis * tiles->GetFrameWidth();
			yShift = yAxis * tiles->GetFrameHeight();
			tileCorners[0] = Point::Zero;
			tileCorners[1] = xShift;
			tileCorners[2] = xShift + yShift;
			tileCorners[3] = yShift;
		}
	}


	bool TileMap::Save(FileSaver& fs)
	{
		AxisShapedGameObject::Save(fs);
		fs.Write(tiles);
		fs.Write(tint);

		fs.Write(chunks.size());
		for (auto it = chunks.begin(); it != chunks.end(); ++it) {
			fs.Write(it->first.x);
			fs.Write(it->first.y);
			uint8_t* arr = it->second.arr;
			for (uint32_t i = 0; i < chunkSize * chunkSize; ++i) {
				fs.Write(arr[i]);
			}
		}

		return fs.GetStatus();
	}


	bool TileMap::Load(FileLoader& fl)
	{
		AxisShapedGameObject::Load(fl);
		TileSet* tileSet;
		fl.Read(&tileSet);
		SetTileSet(tileSet);
		fl.Read(tint);

		size_t size;
		fl.Read(size);
		chunks.clear();
		for (size_t i = 0; i < size; ++i) {
			int32_t x, y;
			fl.Read(x);
			fl.Read(y);
			uint8_t count = 0;
			uint8_t* arr = new uint8_t[chunkSize * chunkSize];
			for (uint32_t j = 0; j < chunkSize * chunkSize; ++j) {
				fl.Read(arr[j]);
				if (arr[j] != 0) {
					++count;
				}
			}
			chunks.insert({ IntPoint(x, y), ChunkData(arr, count) });
		}

		return fl.GetStatus();
	}

}
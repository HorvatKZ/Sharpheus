#include "pch.h"
#include "TileMap.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"
#include "Engine/Renderer/Renderer.hpp"


namespace Sharpheus {

	SPH_START_CLASSINFO(TileMap, "tilemap.png")
		SPH_PROVIDE_LAYER(TileMap, "Layer", GetLayer, SetLayer)
		SPH_PROVIDE_TILESET(TileMap, "TileSet", GetTileSet, SetTileSet, SetTileSetFromPath)
		SPH_PROVIDE_COLOR(TileMap, "Tint", GetTint, SetTint)
		SPH_PROVIDE_TILEMAP("TileMap Editor")
	SPH_END_CLASSINFO

	const int32 TileMap::chunkSize = 16;


	TileMap::TileMap(GameObject* parent, const std::string& name) :
		AxisGameObject(parent, name, new Rect()) {
		prevAxis[0] = Point::Right;
		prevAxis[1] = Point::Up;
	}


	void TileMap::CopyFrom(GameObject* other)
	{
		SPH_CHECKTYPE(other, TileMap);

		AxisGameObject::CopyFrom(other);
		TileMap* trueOther = (TileMap*)other;
		SetTileSet(trueOther->tiles);
		tint = trueOther->tint;
		chunks.clear();
		for (auto it = trueOther->chunks.begin(); it != trueOther->chunks.end(); ++it) {
			auto it2 = chunks.insert({ it->first, ChunkData() }).first;
			it2->second.count = it->second.count;
			memcpy(it2->second.arr, it->second.arr, chunkSize * chunkSize);
		}
	}


	void TileMap::SetTileSetFromPath(const std::string& path)
	{
		SetTileSet(ResourceManager::GetTileSet(path));
	}


	byte TileMap::GetTile(const IntPoint& coord)
	{
		IntPoint chunk = coord.ToChunk();
		auto it = chunks.find(chunk);
		if (it == chunks.end()) {
			return 0;
		}

		return it->second.arr[coord.ToRelInd()];
	}

	void TileMap::SetTile(const IntPoint& coord, byte value)
	{
		if (value == 0) {
			ClearTile(coord);
			return;
		}

		IntPoint chunk = coord.ToChunk();
		auto it = chunks.find(chunk);
		if (it == chunks.end()) {
			it = chunks.insert({ chunk, ChunkData() }).first;
			memset(it->second.arr, 0, chunkSize * chunkSize);
		}
		uint8 relInd = coord.ToRelInd();
		byte oldValue = it->second.arr[relInd];
		if (oldValue == 0) {
			++(it->second.count);
		}
		it->second.arr[relInd] = value;
	}


	void TileMap::ClearTile(const IntPoint& coord)
	{
		IntPoint chunk = coord.ToChunk();
		auto it = chunks.find(chunk);
		if (it == chunks.end()) {
			return;
		}

		uint8 relInd = coord.ToRelInd();
		byte oldValue = it->second.arr[relInd];
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
				byte* arr = it->second.arr;
				for (uint32 i = 0; i < chunkSize * chunkSize; ++i) {
					if (arr[i] != 0) {
						tiles->Render(GetCornersOf(it->first, i), arr[i] - 1, shape->NeedToMirrorX(), tint);
					}
				}
			}
		}
	}


	void TileMap::RenderSelection()
	{
		if (tiles != nullptr) {
			for (auto it = chunks.begin(); it != chunks.end(); ++it) {
				byte* arr = it->second.arr;
				for (uint32 i = 0; i < chunkSize * chunkSize; ++i) {
					if (arr[i] != 0) {
						Renderer::DrawMonocromeQuad(GetCornersOf(it->first, i), selectColor);
					}
				}
			}
		}
	}


	bool TileMap::IsSelected(const Point& pos)
	{
		return tiles != nullptr && GetTile(ToTileCoord(pos)) != 0;
	}


	void TileMap::SetBack(byte max)
	{
		for (auto it = chunks.begin(); it != chunks.end(); ++it) {
			byte* arr = it->second.arr;
			for (uint32 i = 0; i < chunkSize * chunkSize; ++i) {
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
		AxisGameObject::Save(fs);
		fs.Write(tiles);
		fs.Write(tint);

		fs.Write(chunks.size());
		for (auto it = chunks.begin(); it != chunks.end(); ++it) {
			fs.Write(it->first.x);
			fs.Write(it->first.y);
			byte* arr = it->second.arr;
			for (uint32 i = 0; i < chunkSize * chunkSize; ++i) {
				fs.Write(arr[i]);
			}
		}

		return fs.GetStatus();
	}


	bool TileMap::Load(FileLoader& fl)
	{
		AxisGameObject::Load(fl);
		const TileSet* tileSet;
		fl.Read(&tileSet);
		SetTileSet(tileSet);
		fl.Read(tint);

		size_t size;
		fl.Read(size);
		chunks.clear();
		for (size_t i = 0; i < size; ++i) {
			int32 x, y;
			fl.Read(x);
			fl.Read(y);
			uint8 count = 0;
			byte* arr = new byte[chunkSize * chunkSize];
			for (uint32 j = 0; j < chunkSize * chunkSize; ++j) {
				fl.Read(arr[j]);
				if (arr[j] != 0) {
					++count;
				}
			}
			chunks.insert({ IntPoint(x, y), ChunkData(arr, count) });
		}

		UpdateSizer();
		return fl.GetStatus();
	}

}
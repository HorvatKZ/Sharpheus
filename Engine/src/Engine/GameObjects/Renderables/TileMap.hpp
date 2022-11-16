#pragma once

#include "../AxisGameObject.hpp"
#include "Engine/ResourceManager/TileSet.hpp"


namespace Sharpheus {

	class SPH_EXPORT TileMap : public AxisGameObject
	{
	public:
		struct IntPoint {
			int32 x, y;

			IntPoint(int32 x, int32 y) : x(x), y(y) {}
			IntPoint(float x, float y) : x(glm::floor(x)), y(glm::floor(y)) {}

			bool operator==(const IntPoint& other) const {
				return x == other.x && y == other.y;
			}

			inline IntPoint ToChunk() const { return IntPoint(DivByChunkSize(x), DivByChunkSize(y)); }
			inline IntPoint ToRelPos() const { return IntPoint(ModByChunkSize(x), ModByChunkSize(y)); }
			inline uint8 ToRelInd() const { return ModByChunkSize(y) * chunkSize + ModByChunkSize(x); }

			static inline int32 DivByChunkSize(int32 a) { return (a >= 0) ? (a / chunkSize) : ((a + 1) / chunkSize - 1); }
			static inline int32 ModByChunkSize(int32 a) { return (a % chunkSize == 0) ? 0 : ((a >= 0) ? (a % chunkSize) : (a % chunkSize + chunkSize)); }
		};

		TileMap(GameObject* parent, const std::string& name);
		virtual ~TileMap() = default;
		virtual void CopyFrom(GameObject* other) override;

		inline const TileSet* GetTileSet() { return tiles; }
		inline const Color& GetTint() { return tint; }
		inline void SetTint(const Color& tint) { this->tint = tint; }
		inline void SetTileSet(const TileSet* tiles) {
			if (tiles != nullptr && this->tiles != nullptr && tiles->GetNumOfTiles() < this->tiles->GetNumOfTiles()) {
				SetBack(tiles->GetNumOfTiles());
			}
			this->tiles = tiles;
			UpdateSizer();
			UpdateTileCorners();
		}

		void SetTileSetFromPath(const std::string& path);

		inline IntPoint ToTileCoord(const Point& pos) {
			if (tiles == nullptr) {
				return IntPoint(0, 0);
			}

			Point localPos = Transform(pos).SubstractFirst(worldTrafo).pos;
			return IntPoint(localPos.x / tiles->GetFrameWidth(), localPos.y / tiles->GetFrameHeight());
		}

		byte GetTile(const IntPoint& coord);
		void SetTile(const IntPoint& coord, byte value);
		void ClearTile(const IntPoint& coord);

		virtual void Render() override;

		virtual bool Load(FileLoader& fl) override;

		SPH_DECL_GAMEOBJECT(TileMap)

	protected:
		struct ChunkData {
			byte* arr;
			uint8 count = 0;

			ChunkData() : arr(new byte[chunkSize * chunkSize]) {}
			ChunkData(byte* arr, uint8 count) : arr(arr), count(count) {}
			ChunkData(ChunkData&& other) noexcept : arr(other.arr), count(other.count) { other.arr = nullptr; }
			ChunkData(const ChunkData& other) = default;
			ChunkData& operator=(const ChunkData& other) = default;
			~ChunkData() { delete[] arr; }
		};

		struct IntPointHasher {
			std::size_t operator() (const IntPoint& key) const {
				std::size_t hash = key.x;
				return (hash << 32) + key.y;
			}
		};

		const TileSet* tiles = nullptr;
		Color tint = Color::White;
		std::unordered_map<IntPoint, ChunkData, IntPointHasher> chunks;
		Point tileCorners[4], tempCorners[4], prevAxis[2], xShift, yShift;
		static const int32 chunkSize;

		virtual bool Save(FileSaver& fs) override;

		virtual void RenderSelection() override;
		virtual bool IsSelected(const Point& pos) override;

		void SetBack(byte max);
		void UpdateTileCorners();
		void UpdateSizer() override {}
		inline Point* GetCornersOf(const IntPoint& chunk, uint8 ind) {
			Point shift = worldTrafo.pos + (chunk.x * chunkSize + ind % chunkSize) * xShift + (chunk.y * chunkSize + ind / chunkSize) * yShift;
			tempCorners[0] = tileCorners[0] + shift;
			tempCorners[1] = tileCorners[1] + shift;
			tempCorners[2] = tileCorners[2] + shift;
			tempCorners[3] = tileCorners[3] + shift;
			return tempCorners;
		}
	};

}


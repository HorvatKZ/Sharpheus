#pragma once

#include "../AxisShapedGameObject.hpp"
#include "Engine/ResourceManager/TileSet.hpp"


namespace Sharpheus {

	class SPH_EXPORT TileMap : public AxisShapedGameObject
	{
	public:
		struct IntPoint {
			int32_t x, y;

			IntPoint(int32_t x, int32_t y) : x(x), y(y) {}
			IntPoint(float x, float y) : x(glm::floor(x)), y(glm::floor(y)) {}

			bool operator==(const IntPoint& other) const {
				return x == other.x && y == other.y;
			}

			inline IntPoint ToChunk() const { return IntPoint(DivByChunkSize(x), DivByChunkSize(y)); }
			inline IntPoint ToRelPos() const { return IntPoint(ModByChunkSize(x), ModByChunkSize(y)); }
			inline uint8_t ToRelInd() const { return ModByChunkSize(y) * chunkSize + ModByChunkSize(x); }

			inline int32_t DivByChunkSize(int32_t a) const { return (a >= 0) ? (a / chunkSize) : (a / chunkSize - 1); }
			inline int32_t ModByChunkSize(int32_t a) const { return (a >= 0) ? (a % chunkSize) : (a % chunkSize + chunkSize); }
		};

		struct ChunkData {
			uint8_t* arr;
			uint8_t count = 0;

			ChunkData() : arr(new uint8_t[chunkSize * chunkSize]) {}
			ChunkData(uint8_t* arr, uint8_t count) : arr(arr), count(count) {}
			ChunkData(ChunkData&& other) : arr(other.arr), count(other.count) { other.arr = nullptr; }
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

		TileMap(GameObject* parent, const std::string& name);
		virtual ~TileMap() = default;
		virtual void CopyFrom(GameObject* other) override;

		inline TileSet* GetTileSet() { return tiles; }
		inline const Color& GetTint() { return tint; }
		inline void SetTint(const Color& tint) { this->tint = tint; }
		inline void SetTileSet(TileSet* tiles) {
			if (tiles != nullptr && this->tiles != nullptr && tiles->GetNumOfTiles() < this->tiles->GetNumOfTiles()) {
				SetBack(tiles->GetNumOfTiles());
			}
			this->tiles = tiles;
			UpdateSizer();
			UpdateTileCorners();
		}

		void SetTileSetFromPath(const std::string& path);

		inline IntPoint ToTile(const Point& pos) {
			if (tiles == nullptr) {
				return IntPoint(0, 0);
			}

			Point localPos = Transform(pos).SubstractFirst(worldTrafo).pos;
			return IntPoint(localPos.x / tiles->GetFrameWidth(), localPos.y / tiles->GetFrameHeight());
		}

		uint8_t Get(const IntPoint& pos);
		void Set(const IntPoint& pos, uint8_t value);
		void Clear(const IntPoint& pos);

		virtual bool Load(FileLoader& fl) override;

		SPH_DECL_GAMEOBJECT(TileMap)

	protected:
		TileSet* tiles = nullptr;
		Color tint = Color::White;
		std::unordered_map<IntPoint, ChunkData, IntPointHasher> chunks;
		Point tileCorners[4], tempCorners[4], prevAxis[2], xShift, yShift;
		static const int32_t chunkSize;

		virtual bool Save(FileSaver& fs) override;

		virtual void Render() override;

		virtual void RenderSelection() override;
		virtual bool IsSelected(const Point& pos) override;

		void SetBack(uint8_t max);
		void UpdateTileCorners();
		void UpdateSizer() override {}
		inline Point* GetCornersOf(const IntPoint& chunk, uint8_t ind) {
			Point shift = worldTrafo.pos + (chunk.x * chunkSize + ind % chunkSize) * xShift + (chunk.y * chunkSize + ind / chunkSize) * yShift;
			tempCorners[0] = tileCorners[0] + shift;
			tempCorners[1] = tileCorners[1] + shift;
			tempCorners[2] = tileCorners[2] + shift;
			tempCorners[3] = tileCorners[3] + shift;
			return tempCorners;
		}
	};

}


#pragma once

#include "Image.hpp"

namespace Sharpheus {

	class SPH_EXPORT Animation : public Resource
	{
	public:
		Animation(const std::string& animFile);
		Animation(Image* atlas, uint32_t frameWidth, uint32_t frameHeight, uint32_t startFrame, uint32_t endFrame);
		virtual ~Animation();

		inline const std::string& GetName() { return name; }
		inline Image* GetAtlas() { return atlas; }
		inline uint32_t GetFrameWidth() { return frameWidth; }
		inline uint32_t GetFrameHeight() { return frameHeight; }
		inline uint32_t GetFrameRows() { return frameRows; }
		inline uint32_t GetFrameCols() { return frameCols; }
		inline uint32_t GetStartFrame() { return startFrame; }
		inline uint32_t GetEndFrame() { return endFrame; }
		inline float GetFrameTime() { return frameTime; }
		inline uint32_t GetNumOfFrames() { return endFrame - startFrame + 1; }
		inline uint32_t GetNumOfAllFrames() { return frameRows * frameCols; }
		inline float GetFullTime() { return GetNumOfFrames() * frameTime; }

		inline void SetFrameTime(float frameTime) { this->frameTime = frameTime; }
		inline void SetName(const std::string& name) { this->name = name; }

		void Render(Point coords[4], float time, const Color& tint);

		bool Save();
		bool Save(const std::string& path);

	private:
		std::string name;
		Image* atlas;
		uint32_t frameWidth, frameHeight, frameCols, frameRows;
		uint32_t startFrame, endFrame;
		float frameTime = 1.0f;
		Point* texCoords = nullptr;

		bool Load();
		void CalcTexCoords();
	};

}
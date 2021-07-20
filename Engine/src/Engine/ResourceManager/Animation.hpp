#pragma once

#include "Image.hpp"

namespace Sharpheus {

	class SPH_EXPORT Animation : public Resource
	{
	public:
		Animation(const std::string& animFile);
		Animation(Image* atlas, uint32 frameWidth, uint32 frameHeight, uint32 startFrame, uint32 endFrame);
		virtual ~Animation();

		inline const std::string& GetName() { return name; }
		inline Image* GetAtlas() { return atlas; }
		inline uint32 GetFrameWidth() { return frameWidth; }
		inline uint32 GetFrameHeight() { return frameHeight; }
		inline uint32 GetFrameRows() { return frameRows; }
		inline uint32 GetFrameCols() { return frameCols; }
		inline uint32 GetStartFrame() { return startFrame; }
		inline uint32 GetEndFrame() { return endFrame; }
		inline float GetFrameTime() { return frameTime; }
		inline uint32 GetNumOfFrames() { return endFrame - startFrame + 1; }
		inline uint32 GetNumOfAllFrames() { return frameRows * frameCols; }
		inline float GetFullTime() { return GetNumOfFrames() * frameTime; }

		inline void SetFrameTime(float frameTime) { this->frameTime = frameTime; }
		inline void SetName(const std::string& name) { this->name = name; }

		void Render(Point coords[4], float time, const Color& tint);

		bool Save();
		bool Save(const std::string& path);

	private:
		std::string name;
		Image* atlas;
		uint32 frameWidth, frameHeight, frameCols, frameRows;
		uint32 startFrame, endFrame;
		float frameTime = 1.0f;
		Point* texCoords = nullptr;

		bool Load();
		void CalcTexCoords();
	};

}
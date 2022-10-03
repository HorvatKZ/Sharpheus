#pragma once

#include "Image.hpp"

namespace Sharpheus {

	class SPH_EXPORT Animation : public Resource
	{
	public:
		Animation(const std::string& animFile);
		Animation(const Image* atlas, uint32 frameWidth, uint32 frameHeight, uint32 startFrame, uint32 endFrame);
		virtual ~Animation();

		inline const std::string& GetName() const { return name; }
		inline const Image* GetAtlas() const { return atlas; }
		inline uint32 GetFrameWidth() const { return frameWidth; }
		inline uint32 GetFrameHeight() const { return frameHeight; }
		inline uint32 GetFrameRows() const { return frameRows; }
		inline uint32 GetFrameCols() const { return frameCols; }
		inline uint32 GetStartFrame() const { return startFrame; }
		inline uint32 GetEndFrame() const { return endFrame; }
		inline float GetFrameTime() const { return frameTime; }
		inline uint32 GetNumOfFrames() const { return endFrame - startFrame + 1; }
		inline uint32 GetNumOfAllFrames() const { return frameRows * frameCols; }
		inline float GetFullTime() const { return GetNumOfFrames() * frameTime; }

		inline void SetFrameTime(float frameTime) { this->frameTime = frameTime; }
		inline void SetName(const std::string& name) { this->name = name; }

		void Render(const Point coords[4], float time, bool mirrorX = false, const Color& tint = Color::White) const;

		bool Save();
		bool Save(const std::string& path);

	private:
		std::string name;
		const Image* atlas;
		uint32 frameWidth, frameHeight, frameCols, frameRows;
		uint32 startFrame, endFrame;
		float frameTime = 1.0f;
		Point* texCoords = nullptr;

		bool Load();
		void CalcTexCoords();
	};

}
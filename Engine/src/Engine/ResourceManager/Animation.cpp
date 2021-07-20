#include "pch.h"
#include "Animation.hpp"
#include "Engine/FileUtils/FileLoader.hpp"
#include "Engine/FileUtils/FileSaver.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"


namespace Sharpheus {

	Animation::Animation(const std::string& animFile)
		: Resource(animFile)
	{
		valid = Load();
	}


	Animation::Animation(Image* atlas, uint32 frameWidth, uint32 frameHeight, uint32 startFrame, uint32 endFrame)
		: Resource(), atlas(atlas), frameWidth(frameWidth), frameHeight(frameHeight), startFrame(startFrame), endFrame(endFrame)
	{
		if (atlas != nullptr) {
			frameCols = atlas->GetWidth() / frameWidth;
			frameRows = atlas->GetHeight() / frameHeight;
			CalcTexCoords();
			valid = atlas->IsValid();
		} else {
			SPH_ERROR("The texture atlas must be set");
		}
	}


	Animation::~Animation()
	{
		delete[] texCoords;
	}


	void Animation::Render(Point coords[4], float time, const Color& tint)
	{
		time = glm::clamp(time, 0.f, GetFullTime());

		uint32 ind = time / frameTime;

		atlas->RenderPart(coords, texCoords + 4 * ind, tint);
	}


	bool Animation::Save()
	{
		SPH_ASSERT(HasPath(), "No path set to animation");
		return Save(fullPath);
	}


	bool Animation::Save(const std::string& path)
	{
		SetPath(path);

		FileSaver fs(fullPath);
		SPH_ASSERT(fs.GetStatus(), "Cannot open the location: {0}", fullPath);

		fs.Write(name);
		fs.Write(atlas);
		fs.Write(frameWidth);
		fs.Write(frameHeight);
		fs.Write(startFrame);
		fs.Write(endFrame);
		fs.Write(frameTime);
		return fs.GetStatus();
	}


	bool Animation::Load()
	{
		FileLoader fl(fullPath);
		SPH_ASSERT(fl.GetStatus(), "Cannot open the location: {0}", fullPath);

		fl.Read(name);
		fl.Read(&atlas);
		fl.Read(frameWidth);
		fl.Read(frameHeight);
		fl.Read(startFrame);
		fl.Read(endFrame);
		fl.Read(frameTime);

		if (atlas == nullptr) {
			SPH_ERROR("The animation atlas from {0} is not valid!", fullPath);
			return false;
		}

		frameCols = atlas->GetWidth() / frameWidth;
		frameRows = atlas->GetHeight() / frameHeight;
		CalcTexCoords();
		return fl.GetStatus() && atlas->IsValid();
	}


	void Animation::CalcTexCoords()
	{
		texCoords = new Point[(endFrame - startFrame + 1) * 4];
		for (uint32 frame = startFrame, ind = 0; frame <= endFrame; ++frame) {
			float xStart = ((frame % frameCols) * frameWidth) / (float)atlas->GetWidth();
			float yStart = ((frame / frameCols) * frameHeight) / (float)atlas->GetHeight();
			float xEnd = xStart + frameWidth / (float)atlas->GetWidth();
			float yEnd = yStart + frameHeight / (float)atlas->GetHeight();
			texCoords[ind * 4] = Point(xStart, yStart);
			texCoords[ind * 4 + 1] = Point(xEnd, yStart);
			texCoords[ind * 4 + 2] = Point(xEnd, yEnd);
			texCoords[ind * 4 + 3] = Point(xStart, yEnd);
			++ind;
		}
	}
}
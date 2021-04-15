#include "pch.h"
#include "ProjectControl.hpp"


namespace Sharpheus {

	std::string ProjectControl::loadLevelPath;
	bool ProjectControl::needToExit = false;


	void ProjectControl::Init()
	{
	}


	void ProjectControl::Clear()
	{
	}


	void ProjectControl::LoadLevel(const std::string& path)
	{
		loadLevelPath = path;
	}


	void ProjectControl::Exit()
	{
		needToExit = true;
	}


	std::string ProjectControl::GetLoadLevelPath()
	{
		std::string result = loadLevelPath;
		loadLevelPath.clear();
		return result;
	}


	bool ProjectControl::NeedToExit()
	{
		return needToExit;
	}

	void ProjectControl::ResetExit()
	{
		needToExit = false;
	}
}
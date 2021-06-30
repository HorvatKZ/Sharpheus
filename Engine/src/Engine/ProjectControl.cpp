#include "pch.h"
#include "ProjectControl.hpp"


namespace Sharpheus {

	std::string ProjectControl::loadLevelPath;
	bool ProjectControl::needToExit = false;
	bool ProjectControl::isPaused = false;


	void ProjectControl::Init()
	{
		isPaused = false;
		needToExit = false;
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


	void ProjectControl::Pause()
	{
		isPaused = true;
	}


	void ProjectControl::Resume()
	{
		isPaused = false;
	}


	std::string ProjectControl::GetLoadLevelPath()
	{
		std::string result = loadLevelPath;
		loadLevelPath.clear();
		return result;
	}

}
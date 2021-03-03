#include "editor_pch.h"
#include "ProjectData.hpp"


namespace Sharpheus {

	Project* ProjectData::proj = nullptr;

	void ProjectData::InitNew(const std::string& name, const std::string& path, const std::string& levelName ,const std::string& levelPath)
	{
		Clear();

		proj = new Project(name, path, levelName, levelPath);
	}


	void ProjectData::Init(const std::string& path)
	{
		Clear();

		proj = new Project();
		SPHE_LOG("Opening project/level: {0}", path);
		bool success = proj->Load(path);
		SPHE_ASSERT(success, "Cannot load project/level");
	}


	void ProjectData::Clear()
	{
		delete proj;
	}

}
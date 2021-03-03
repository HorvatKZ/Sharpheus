#include "pch.h"
#include "Project.hpp"
#include "FileUnits/FileLoader.hpp"
#include "FileUnits/FileSaver.hpp"
#include "ResourceManager/ResourceManager.hpp"


namespace Sharpheus {

	Project::Project()
	{
	}


	Project::Project(const std::string& name, const std::string& path, const std::string& defaultLevelName, const std::string& defaultLevelPath)
		: name(name), path(path), defaultLevelPath(defaultLevelPath)
	{
		size_t pos = path.find_last_of('\\'); // windows only
		basePath = path.substr(0, pos + 1);
		ResourceManager::Init(basePath);

		level = new Level(defaultLevelName);
		level->SetProjectPath(path);
		level->Save(basePath + "Levels\\", defaultLevelPath);
		SetWinProps(winProps);
		SaveProjectData();	
	}


	Project::~Project()
	{
		delete level;
	}


	bool Project::LoadLevel(const std::string& path)
	{
		if (level == nullptr) {
			level = new Level();
		}
		
		bool success = level->Load(basePath + "Levels\\", path);
		level->SetProjectPath(this->path);
		return success;
	}


	bool Project::SaveLevel(const std::string& path)
	{
		return level->Save(basePath + "Levels\\", path);;
	}


	void Project::CreateNewLevel(const std::string& name)
	{
		delete level;
		level = new Level(name);
		level->SetProjectPath(path);
	}


	bool Project::Load(const std::string& path)
	{
		size_t pos = path.find_last_of('.');
		if (pos == std::string::npos || path.substr(pos + 1) != "sharpheus") {
			SPH_ERROR("Cannot load project from \"{0}\". Non-correct file type. The correct file type is *.proj.sharpheus", path);
			return false;
		}

		std::string noExtensionPath = path.substr(0, pos);
		pos = noExtensionPath.find_last_of('.');
		std::string type = noExtensionPath.substr(pos + 1);

		if (type == "proj") {
			bool success = true;
			success &= LoadProjectData(path);
			level = new Level();
			success &= level->Load(basePath + "Levels\\", defaultLevelPath);
			level->SetProjectPath(path);
			return success;
		}
		else if (type == "lvl") {
			bool success = true;
			level = new Level();
			success &= level->LoadLevelData(path);
			success &= LoadProjectData(level->GetProjectPath());
			std::string base = basePath + "Levels\\";
			std::string relativePath = path.substr(base.length());
			success &= level->Load(base, relativePath);
			return success;
		}
		
		SPH_ERROR("Currently only *.proj.shaprheus and *.lvl.sharpheus files are supported at project creation");
		return false;
	}


	bool Project::SaveProjectData()
	{
		bool success = true;
		FileSaver fs(path);
		success &= fs.Write(name);
		success &= fs.WriteEnd();
		success &= fs.Write(winProps.title);
		success &= fs.Write(winProps.width);
		success &= fs.Write(winProps.height);
		success &= fs.Write(winProps.fullscreen);
		success &= fs.Write(winProps.vsync);
		success &= fs.Write(winProps.background);
		success &= fs.WriteEnd();
		success &= fs.Write(defaultLevelPath);
		success &= fs.WriteEnd();
		SPH_ASSERT(success, "Cannot save project data to \"{0}\"", path);
		return success;
	}


	void Project::SetWinProps(const WinProps& winProps)
	{
		this->winProps = winProps;
		Renderer::SetBackgroundColor(winProps.background);
		Camera::SetStaticRect(winProps.width, winProps.height);
	}


	bool Project::LoadProjectData(const std::string& path)
	{
		size_t pos = path.find_last_of('\\'); // windows only
		basePath = path.substr(0, pos + 1);
		ResourceManager::Init(basePath);

		this->path = path;
		bool success = true;
		FileLoader fl(path);
		success &= fl.Read(name);
		success &= fl.TryReadingEnd();
		success &= fl.Read(winProps.title);
		success &= fl.Read(winProps.width);	
		success &= fl.Read(winProps.height);	
		success &= fl.Read(winProps.fullscreen);	
		success &= fl.Read(winProps.vsync);
		success &= fl.Read(winProps.background);
		success &= fl.TryReadingEnd();
		success &= fl.Read(defaultLevelPath);
		success &= fl.TryReadingEnd();
		SetWinProps(winProps);
		SPH_ASSERT(success, "Cannot read project data from \"{0}\"", path);
		return success;
	}
}
#include "pch.h"
#include "Project.hpp"
#include "ProjectControl.hpp"
#include "FileUtils/FileLoader.hpp"
#include "FileUtils/FileSaver.hpp"
#include "ResourceManager/ResourceManager.hpp"


namespace Sharpheus {

	uint32 Project::timeOfRun = 0;


	Project::Project()
	{
		ProjectControl::Init();
	}


	Project::Project(const Data& data, const std::string& basePath, const std::string& fileName)
		: data(data), basePath(basePath), fileName(fileName)
	{
		ProjectControl::Init();

		if (basePath[basePath.size() - 1] != '\\') {
			this->basePath += '\\';
		}
		ResourceManager::Init(this->basePath);
		SetWinProps(data.winProps);

		LoadLevel(data.defaultLevelPath);
	}


	Project::Project(const std::string& name, const std::string& path, const std::string& defaultLevelName, const std::string& defaultLevelPath)
		: path(path)
	{
		ProjectControl::Init();

		data.name = name;
		data.defaultLevelPath = defaultLevelPath;
		data.version = EngineVersion::latest;
		SetWinProps(data.winProps);

		size_t pos = path.find_last_of('\\');
		basePath = path.substr(0, pos + 1);
		ResourceManager::Init(basePath);

		fileName = path.substr(pos + 1);
		level = new Level(defaultLevelName);
		level->SetProjectFileName(fileName);
		level->Save(basePath + "Levels\\", defaultLevelPath);
		SaveProjectData();	
	}


	Project::~Project()
	{
		ProjectControl::Clear();
		if (!isSubProject) {
			ResourceManager::Clear();
		}
		delete level;
	}


	bool Project::LoadLevel(const std::string& path)
	{
		if (level == nullptr) {
			level = new Level();
		}
		
		bool success = level->Load(basePath + "Levels\\", path);
		level->SetProjectFileName(this->fileName);
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
		level->SetProjectFileName(fileName);
	}


	void Project::Tick(float allTime)
	{
		std::string loadLevel = ProjectControl::GetLoadLevelPath();
		if (!loadLevel.empty()) {
			level->Load(basePath + "Levels\\", loadLevel);
			ProjectControl::Resume();
		}
		
		if (!ProjectControl::IsPaused()) {
			level->Tick(allTime - lastTime);
		}
		lastTime = allTime;
		timeOfRun = allTime;
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
			success &= level->Load(basePath + "Levels\\", data.defaultLevelPath);
			level->SetProjectFileName(path.substr(basePath.length() + 7));
			return success;
		}
		else if (type == "lvl") {
			bool success = true;
			level = new Level();
			success &= level->LoadLevelData(path);
			std::string projPath = path.substr(0, path.find_last_of("Level\\"));
			success &= LoadProjectData(projPath + level->GetProjectFileName());
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
		success &= fs.Write(data.name);
		success &= fs.WriteEnd();
		success &= fs.Write(data.winProps.title);
		success &= fs.Write(data.winProps.width);
		success &= fs.Write(data.winProps.height);
		success &= fs.Write(data.winProps.fullscreen);
		success &= fs.Write(data.winProps.vsync);
		success &= fs.Write(data.winProps.background);
		success &= fs.WriteEnd();
		success &= fs.Write(data.defaultLevelPath);
		success &= fs.WriteEnd();
		success &= fs.Write(data.version.num);
		success &= fs.Write(data.version.str);
		success &= fs.WriteEnd();
		SPH_ASSERT(success, "Cannot save project data to \"{0}\"", path);
		return success;
	}


	bool Project::SaveProjectDataToHpp(const std::string& path)
	{
		bool success = true;
		std::ofstream f(path);
		success &= !f.fail();
		f << "#pragma once\n";
		f << "\n";
		f << "#include \"Project.hpp\"\n";
		f << "\n";
		std::string defLevel = data.defaultLevelPath;
		for (char& character : defLevel) {
			if (character == '\\') {
				character = '/';
			}
		}
		f << "namespace Sharpheus {\n";
		f << "Project::Data projectData(\"" << data.name << "\", \"" << defLevel << "\", "
			<< "WinProps(\"" << data.name << "\", " << data.winProps.width << ", " << data.winProps.height << ", "
			<< data.winProps.fullscreen << ", " << data.winProps.vsync << ", Color(" << (uint32)data.winProps.background.r << ", "
			<< (uint32)data.winProps.background.g << ", " << (uint32)data.winProps.background.b << ")), EngineVersion(" <<
			data.version.num << ", " << data.version.str << "));\n";
		f << "}\n";
		f.close();
		SPH_ASSERT(success, "Cannot save project data to \"{0}\"", path);
		return success;
	}


	void Project::SetWinProps(const WinProps& winProps)
	{
		data.winProps = winProps;
		Renderer::SetBackgroundColor(winProps.background);
		Camera::SetStaticRect(winProps.width, winProps.height);
		Camera::SetOGHeight(winProps.height);
	}


	bool Project::LoadProjectData(const std::string& path)
	{
		size_t pos = path.find_last_of('\\');
		basePath = path.substr(0, pos + 1);
		ResourceManager::Init(basePath);

		this->path = path;
		bool success = true;
		FileLoader fl(path);
		success &= fl.Read(data.name);
		success &= fl.TryReadingEnd();
		success &= fl.Read(data.winProps.title);
		success &= fl.Read(data.winProps.width);
		success &= fl.Read(data.winProps.height);
		success &= fl.Read(data.winProps.fullscreen);
		success &= fl.Read(data.winProps.vsync);
		success &= fl.Read(data.winProps.background);
		success &= fl.TryReadingEnd();
		success &= fl.Read(data.defaultLevelPath);
		success &= fl.TryReadingEnd();
		success &= fl.Read(data.version.num);
		success &= fl.Read(data.version.str);
		success &= fl.TryReadingEnd();
		SetWinProps(data.winProps);
		SPH_ASSERT(success, "Cannot read project data from \"{0}\"", path);
		return success;
	}

}
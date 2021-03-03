#pragma once

#include "pch.h"
#include "Level.hpp"
#include "Window/Window.hpp"


namespace Sharpheus {

	class SPH_EXPORT Project
	{
	public:
		Project();
		Project(const std::string& name, const std::string& path, const std::string& defaultLevelName, const std::string& defaultLevelPath);
		virtual ~Project();

		bool Load(const std::string& path);
		bool SaveProjectData();
		bool LoadLevel(const std::string& path);
		bool SaveLevel(const std::string& path);
		void CreateNewLevel(const std::string& name);

		inline void Tick(float deltaTime) { level->Tick(deltaTime); }
		inline void Render() { level->Render(); }

		inline const std::string& GetName() const { return name; }
		inline const WinProps& GetWinProps() const { return winProps; }
		inline const std::string& GetPath() const { return path; }
		inline const std::string& GetBasePath() const { return basePath; }
		inline const std::string& GetDefaultLevelPath() const { return defaultLevelPath; }
		inline Level* GetCurrentLevel() const {
			return level;
		}

		void SetWinProps(const WinProps& winProps);
		inline void SetName(const std::string& name) { this->name = name; }
		inline void SetDefaultLevelPath(const std::string& defaultLevelPath) { this->defaultLevelPath = defaultLevelPath; }

	private:
		std::string name;
		Level* level = nullptr;
		WinProps winProps;
		std::string path, basePath, defaultLevelPath;

		bool LoadProjectData(const std::string& path);
	};

}
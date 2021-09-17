#pragma once

#include "pch.h"
#include "Level.hpp"
#include "Window/Window.hpp"


namespace Sharpheus {

	class SPH_EXPORT Project
	{
	public:
		struct Data {
			Data() {}
			Data(const std::string& name, const std::string& defaultLevelPath, const WinProps& winProps)
				: name(name), defaultLevelPath(defaultLevelPath), winProps(winProps) {}

			std::string name, defaultLevelPath;
			WinProps winProps;
		};

		Project();
		Project(const Data& data, const std::string& path);
		Project(const std::string& name, const std::string& path, const std::string& defaultLevelName, const std::string& defaultLevelPath);
		virtual ~Project();

		inline void MarkAsSubProject() { isSubProject = true; }

		bool Load(const std::string& path);
		bool SaveProjectData();
		bool SaveProjectDataToHpp(const std::string& path);
		bool LoadLevel(const std::string& path);
		bool SaveLevel(const std::string& path);
		void CreateNewLevel(const std::string& name);

		void Tick(float allTime);
		inline void Render() { level->Render(); }

		inline const std::string& GetName() const { return data.name; }
		inline const WinProps& GetWinProps() const { return data.winProps; }
		inline const std::string& GetPath() const { return path; }
		inline const std::string& GetBasePath() const { return basePath; }
		inline const std::string& GetDefaultLevelPath() const { return data.defaultLevelPath; }
		inline Level* GetCurrentLevel() const {
			return level;
		}

		void SetWinProps(const WinProps& winProps);
		inline void SetName(const std::string& name) { data.name = name; }
		inline void SetDefaultLevelPath(const std::string& defaultLevelPath) { data.defaultLevelPath = defaultLevelPath; }

		static inline float GetTimeOfRun() { return timeOfRun; }

	private:
		Data data;
		Level* level = nullptr;
		std::string path, basePath;
		float lastTime = 0.f;
		bool isSubProject = false;

		static uint32 timeOfRun;

		bool LoadProjectData(const std::string& path);
	};

}
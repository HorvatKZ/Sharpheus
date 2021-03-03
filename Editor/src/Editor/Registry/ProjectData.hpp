#pragma once

#include "Engine/Project.hpp"


namespace Sharpheus {

	class ProjectData
	{
	public:
		static void InitNew(const std::string& name, const std::string& path, const std::string& levelName, const std::string& levelPath);
		static void Init(const std::string& path);
		static void Clear();

		static inline bool Save() { return proj->SaveProjectData(); }

		static inline Project* GetProj() { return proj; }
		static inline const Window::Props& GetWinProps() { return proj->GetWinProps(); }
		static inline Level* GetLevel() { return proj->GetCurrentLevel(); }
		static inline const std::string& GetPath() { return proj->GetBasePath(); }
		static inline const std::string& GetProjectPath() { return proj->GetPath(); }

		static inline void SetWinProps(const Window::Props& props) { proj->SetWinProps(props); }

		static inline void CreateNewLevel(const std::string& name) { proj->CreateNewLevel(name); }

	private:
		static Project* proj;
	};

}
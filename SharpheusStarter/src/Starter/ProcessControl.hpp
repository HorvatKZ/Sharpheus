#pragma once

#include "starter_pch.h"


namespace Sharpheus {

	class ProcessControl
	{
	public:
		static void Init();
		static void Clear();

		static void OpenFile(const wxString& path);
		static void OpenProj(const wxString& path);
		static void OpenLvl(const wxString& path);

		static void CreateNewProject(const wxString& name, const wxString& level, const wxString& folder, const wxString& ide, bool build);

		static inline void SetProjectCreated(std::function<void(const wxString&)>&& _projectCreated) {
			projectCreated = std::move(_projectCreated);
		}

		static inline const wxString& GetRoot() { return root; }

	private:
		static std::function<void(const wxString&)> projectCreated;
		static wxString root;

		static void OpenWithEditor(class wxFileName& root, const wxString& path);

		static bool CopyFolderContent(const wxString& from, const wxString& to);
		static bool CopyFolderContent(class wxDir& from, const wxString& toUnder);
		static bool CopyItem(const wxString& from, const wxString& to);
		static bool GenerateSolution(const wxString& solutionFolder, const wxString& name, const wxString& ide, const wxString& commonFolder);
		static bool CreateSharpheusFiles(const wxString& root, const wxString& name, const wxString& level);

		static bool WriteBinary(class wxFile& file, uint32_t value);
		static bool WriteBinary(class wxFile& file, bool value);
		static bool WriteBinary(class wxFile& file, uint8_t value);
		static bool WriteBinary(class wxFile& file, float value);
		static bool WriteBinary(class wxFile& file, const wxString& value);
		static bool WriteBinaryEOLN(class wxFile& file);
	};

}
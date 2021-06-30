#include "starter_pch.h"
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/textfile.h>
#include <wx/filefn.h>
#include <wx/dir.h>
#include "ProcessControl.hpp"


namespace Sharpheus {

	std::function<void(const wxString&)> ProcessControl::projectCreated;
	wxString ProcessControl::root;

	void ProcessControl::Init()
	{
		wxFileName binFolder(wxStandardPaths::Get().GetDataDir());
		binFolder.SetName("");
		root = binFolder.GetFullPath();
	}


	void ProcessControl::Clear()
	{
	}


	void ProcessControl::OpenFile(const wxString& path)
	{
		uint32_t pos = path.find_last_of('.');
		wxString ext = path.Mid(pos + 1);
		if (ext != "sharpheus") {
			wxMessageBox(path + " is not a sharpheus file", "Error", wxICON_ERROR | wxOK | wxCENTRE);
			return;
		}

		wxString withoutExt = path.Left(pos);
		pos = withoutExt.find_last_of('.');
		wxString subExt = withoutExt.Mid(pos + 1);
		if (subExt == "proj") {
			OpenProj(path);
		}
		else if (subExt == "lvl") {
			OpenLvl(path);
		}
		else {
			wxMessageBox(path + " is a non-registered sharpheus file", "Error", wxICON_ERROR | wxOK | wxCENTRE);
			return;
		}
	}


	void ProcessControl::OpenProj(const wxString& path)
	{
		wxFileName project(path);
		if (!project.Exists()) {
			wxMessageBox(path + " does not exists", "Error", wxICON_ERROR | wxOK | wxCENTRE);
			return;
		}

		OpenWithEditor(project, path);
	}


	void ProcessControl::OpenLvl(const wxString& path)
	{
		wxFileName level(path);
		if (!level.Exists()) {
			wxMessageBox(path + " does not exists", "Error", wxICON_ERROR | wxOK | wxCENTRE);
			return;
		}

		bool found = false;
		level.SetName("bin");
		level.SetExt("");
		while (level.GetDirCount() > 1 && !found) {
			if (level.Exists()) {
				found = true;
			}
			else {
				level.RemoveLastDir();
			}
		}
		if (level.GetDirCount() == 1) {
			wxMessageBox("Did not found Editor.exe for this level. Try regenerating, and rebuilding the solution", "Error", wxICON_ERROR | wxOK | wxCENTRE);
			return;
		}

		OpenWithEditor(level, path);
	}


	void ProcessControl::CreateNewProject(const wxString& name, const wxString& level, const wxString& folder, const wxString& ide, bool build)
	{
		wxFileName templateProject(wxStandardPaths::Get().GetDataDir());
		templateProject.SetName("Template");
		wxFileName newProject(folder);
		if (!newProject.GetName().empty()) {
			newProject.AppendDir(newProject.GetName());
		}
		newProject.SetName(name);
		if (!CopyFolderContent(templateProject.GetFullPath(), newProject.GetFullPath())) {
			wxMessageBox("Could not copy the template project to " + newProject.GetFullPath(), "Error", wxICON_ERROR | wxOK | wxCENTRE);
			return;
		}

		wxFileName solution = newProject;
		solution.AppendDir(name);
		solution.SetName("Solution");
		wxFileName commonFolder(wxStandardPaths::Get().GetDataDir());
		commonFolder.SetName("Sharpheus");
		commonFolder.RemoveLastDir();
		if (!GenerateSolution(solution.GetFullPath(), name, ide, commonFolder.GetFullPath())) {
			wxMessageBox("Could not generate the " + ide + "solution", "Error", wxICON_ERROR | wxOK | wxCENTRE);
			return;
		}

		if (!CreateSharpheusFiles(newProject.GetFullPath(), name, level)) {
			wxMessageBox("Could not create the default *.sharpheus files", "Error", wxICON_ERROR | wxOK | wxCENTRE);
			return;
		}

		wxString projectPath = newProject.GetFullPath() + "\\" + name + ".proj.sharpheus";
		if (build) {
			if (wxExecute("MSBuild.exe \"" + solution.GetFullPath() + "\\" + name + ".sln\"", wxEXEC_SYNC) != 0) {
				wxMessageBox("Could not build the solution. Maybe the building tool is not added to PATH", "Error", wxICON_ERROR | wxOK | wxCENTRE);
				return;
			}
			else {
				projectCreated(projectPath);
				wxMessageBox("Project is successfully created!", "Success");
				OpenProj(projectPath);
			}
		}
		else {
			wxMessageBox("Project is successfully created! You need to build it before running", "Success");
			projectCreated(projectPath);
		}
	}


	void ProcessControl::OpenWithEditor(wxFileName& root, const wxString& path)
	{
		root.AppendDir("bin");
		root.AppendDir("Debug");
		root.SetName("Editor");
		root.SetExt("exe");
		if (!root.Exists()) {
			wxMessageBox("Did not found " + root.GetFullPath() + ". Try regenerating, and rebuilding the solution", "Error", wxICON_ERROR | wxOK | wxCENTRE);
			return;
		}

		wxExecute(root.GetFullPath() + " \"" + path + "\"");
		wxExit();
	}


	bool ProcessControl::CheckCommonFiles()
	{
		wxString configPath = wxStandardPaths::Get().GetUserConfigDir() + "\\Sharpheus";
		if (!wxDirExists(configPath)) {
			if (!wxMkdir(configPath)) {
				return false;
			}
		}

		configPath += "\\editorConfig.txt";
		if (!wxFileExists(configPath)) {
			wxTextFile file;
			bool success = file.Create(configPath);
			if (success) {
				wxFileName binFolder(wxStandardPaths::Get().GetDataDir());
				binFolder.SetName("");
				file.AddLine(binFolder.GetFullPath());
				file.AddLine("64.000000 2.000000 t 240 240 240 200");
				file.AddLine("t 4.000000 255 32 64 200");
				success &= file.Write();
			}
			return success;
		}

		return true;
	}


	bool ProcessControl::CopyFolderContent(const wxString& from, const wxString& to)
	{
		if (wxFileName::DirExists(to)) {
			int result = wxMessageBox("Directory " + to + " already exists. Overwrite it?", "Warning", wxICON_WARNING | wxYES | wxNO | wxCENTRE);
			if (result == wxNO) {
				return false;
			}

			wxFileName::Rmdir(to, wxPATH_RMDIR_RECURSIVE);
		}

		bool success = true;
		success &= wxFileName::Mkdir(to, wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
		if (success) {
			wxDir dir(from);
			success &= CopyFolderContent(dir, to);
		}
		return success;
	}


	bool ProcessControl::CopyFolderContent(wxDir& from, const wxString& toUnder)
	{
		bool success = true;

		wxString fileName;
		bool found = from.GetFirst(&fileName);

		if (found) {
			wxDir toDir(toUnder);
			success &= CopyItem(from.GetNameWithSep() + fileName, toDir.GetNameWithSep() + fileName);
			found = from.GetNext(&fileName);
		}

		while (found && success) {
			wxDir toDir(toUnder);
			success &= CopyItem(from.GetNameWithSep() + fileName, toDir.GetNameWithSep() + fileName);
			found = from.GetNext(&fileName);
		}

		return success;
	}


	bool ProcessControl::CopyItem(const wxString& from, const wxString& to)
	{
		bool success = true;

		wxFileName file(from);
		wxFileName newFile(to);
		if (wxFileName::DirExists(file.GetFullPath())) {
			success &= wxFileName::Mkdir(newFile.GetFullPath(), wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
			if (success) {
				wxDir dir(file.GetFullPath());
				success &= CopyFolderContent(dir, newFile.GetFullPath());
			}
		}
		else {
			success &= wxCopyFile(file.GetFullPath(), newFile.GetFullPath());
		}

		return success;
	}


	bool ProcessControl::GenerateSolution(const wxString& solutionFolder, const wxString& name, const wxString& ide, const wxString& commonFolder)
	{
		bool success = true;

		wxTextFile premakeInclude;
		wxString correctedCommonFolder = commonFolder;
		correctedCommonFolder.Replace("\\", "/");
		success &= premakeInclude.Create(solutionFolder + "\\premake_include.lua");
		if (success) {
			premakeInclude.AddLine("ProjectName = \"" + name + "\"");
			premakeInclude.AddLine("CommonSource = \"" + correctedCommonFolder + "\"");
			success &= premakeInclude.Write();
		}

		wxTextFile genSlnBat;
		wxString command = "\"" + commonFolder + "\\external\\Premake\\premake5.exe\" --file=\"" + solutionFolder + "\\premake5.lua\" " + ide;
		success &= genSlnBat.Create(solutionFolder + "\\GenSolution_" + ide + ".bat");
		if (success) {
			genSlnBat.AddLine("call " + command);
			genSlnBat.AddLine("PAUSE");
			success &= genSlnBat.Write();
		}

		wxTextFile genExportBat;
		wxString exportingCommand = "\"" + commonFolder + "\\external\\Premake\\premake5.exe\" --file=\"" + solutionFolder + "\\Exported\\premake5.lua\" " + ide;
		success &= genExportBat.Create(solutionFolder + "\\Exported\\GenSolution.bat");
		if (success) {
			genExportBat.AddLine("call " + exportingCommand);
			success &= genExportBat.Write();
		}

		success &= wxExecute(command, wxEXEC_SYNC) == 0;

		return success;
	}


	bool ProcessControl::CreateSharpheusFiles(const wxString& root, const wxString& name, const wxString& level)
	{
		bool success = true;

		wxFile proj;
		wxString projPath = root + "\\" + name + ".proj.sharpheus";
		success &= proj.Open(projPath, wxFile::write);
		success &= WriteBinary(proj, name);
		success &= WriteBinaryEOLN(proj);
		success &= WriteBinary(proj, name);
		success &= WriteBinary(proj, (uint32_t)1280);
		success &= WriteBinary(proj, (uint32_t)720);
		success &= WriteBinary(proj, false);
		success &= WriteBinary(proj, false);
		success &= WriteBinary(proj, (uint8_t)32);
		success &= WriteBinary(proj, (uint8_t)64);
		success &= WriteBinary(proj, (uint8_t)128);
		success &= WriteBinary(proj, (uint8_t)255);
		success &= WriteBinaryEOLN(proj);
		success &= WriteBinary(proj, level + ".lvl.sharpheus");
		success &= WriteBinaryEOLN(proj);
		success &= proj.Close();

		wxFile lvl;
		success &= lvl.Open(root + "\\Levels\\" + level + ".lvl.sharpheus", wxFile::write);
		success &= WriteBinary(lvl, level);
		success &= WriteBinary(lvl, projPath);
		success &= WriteBinaryEOLN(lvl);
		success &= WriteBinary(lvl, (uint8_t)1);
		success &= WriteBinary(lvl, level);
		success &= WriteBinary(lvl, (uint32_t)1);
		success &= WriteBinary(lvl, 0.f);
		success &= WriteBinary(lvl, 0.f);
		success &= WriteBinary(lvl, 1.f);
		success &= WriteBinary(lvl, 1.f);
		success &= WriteBinary(lvl, 0.f);
		success &= WriteBinary(lvl, (uint8_t)1);
		success &= WriteBinaryEOLN(lvl);
		success &= WriteBinary(lvl, (uint8_t)16);
		success &= WriteBinary(lvl, wxString("Camera"));
		success &= WriteBinary(lvl, (uint32_t)0);
		success &= WriteBinary(lvl, 0.f);
		success &= WriteBinary(lvl, 0.f);
		success &= WriteBinary(lvl, 1.f);
		success &= WriteBinary(lvl, 1.f);
		success &= WriteBinary(lvl, 0.f);
		success &= WriteBinary(lvl, (uint8_t)1);
		success &= WriteBinary(lvl, true);
		success &= WriteBinaryEOLN(lvl);
		success &= lvl.Close();

		return success;
	}

	bool ProcessControl::WriteBinary(wxFile& file, uint32_t value)
	{
		return file.Write(&value, sizeof(value)) == sizeof(value);
	}

	bool ProcessControl::WriteBinary(wxFile& file, bool value)
	{
		uint8_t realValue = value ? 1 : 0;
		return WriteBinary(file, realValue);
	}

	bool ProcessControl::WriteBinary(wxFile& file, uint8_t value)
	{
		return file.Write(&value, sizeof(value)) == sizeof(value);
	}

	bool ProcessControl::WriteBinary(wxFile& file, float value)
	{
		return file.Write(&value, sizeof(value)) == sizeof(value);
	}

	bool ProcessControl::WriteBinary(wxFile& file, const wxString& value)
	{
		bool success = true;
		uint32_t len = value.Length();
		success &= file.Write(&len, sizeof(len)) == sizeof(len);
		success &= file.Write(value.c_str(), len + 1) == len + 1;
		return success;
	}

	bool ProcessControl::WriteBinaryEOLN(wxFile& file)
	{
		uint8_t eoln = '\n';
		return WriteBinary(file, eoln);
	}

}
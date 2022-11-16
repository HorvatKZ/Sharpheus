#include "editor_pch.h"
#include <wx/dir.h>
#include <wx/filename.h>
#include "Exporter.hpp"
#include "Editor/Registry/ProjectData.hpp"
#include "Engine/PythonInterface/PythonInterface.hpp"


namespace Sharpheus {

	Exporter::Exporter(bool includeLogging) :includeLogging(includeLogging)
	{
	}

	Exporter::~Exporter()
	{
	}

	void Exporter::Export()
	{
		wxString exportedFolder = ProjectData::GetPath() + "Exported";
		if (wxFileName::DirExists(exportedFolder)) {
			int result = wxMessageBox("The Exported folder is not empty. This operation will overwrite its content. Proceed?", "Warning", wxICON_WARNING | wxYES | wxNO | wxCENTRE);
			if (result == wxNO) {
				return;
			}

			wxFileName::Rmdir(exportedFolder, wxPATH_RMDIR_RECURSIVE);
		}

		bool success = true;
		success &= wxFileName::Mkdir(exportedFolder, wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
		if (!success) {
			wxMessageBox("Could not create Exported folder", "Exporting error", wxICON_ERROR | wxOK | wxCENTRE);
			return;
		}

		success &= ProjectData::GetProj()->SaveProjectDataToHpp(ProjectData::GetPath() + "Source\\ExportedProjectData.hpp");
		if (!success) {
			wxMessageBox("Could not export project data", "Exporting error", wxICON_ERROR | wxOK | wxCENTRE);
			return;
		}

		success &= CopyFolderContent("Assets");
		success &= CopyFolderContent("Levels");
		success &= CopyFolderContent("Scenes");
		if (!success) {
			wxMessageBox("Could not copy resource folders", "Exporting error", wxICON_ERROR | wxOK | wxCENTRE);
			return;
		}

		if (!PrecompilePyScripts()) {
			wxMessageBox("Could not precompile python scripts", "Exporting error", wxICON_ERROR | wxOK | wxCENTRE);
			return;
		}

		if (wxExecute("\"" + ProjectData::GetPath() + "Solution\\Exported\\GenSolution.bat\" -nopause", wxEXEC_SYNC) != 0) {
			wxMessageBox("Could not generate Exported project", "Exporting error", wxICON_ERROR | wxOK | wxCENTRE);
			return;
		}

		wxString config = includeLogging ? "Release" : "Final";
		if (wxExecute("MSBuild.exe \"" + ProjectData::GetPath() + "Solution\\Exported\\Exported.sln\" /property:Configuration=" + config, wxEXEC_SYNC) != 0) {
			wxMessageBox("Could not build Exported project", "Exporting error", wxICON_ERROR | wxOK | wxCENTRE);
			return;
		}

		if (!wxRenameFile(ProjectData::GetPath() + "Exported\\Exported.exe",
			ProjectData::GetPath() + "Exported\\" + ProjectData::GetProj()->GetName() + ".exe")) {
			wxMessageBox("Could not rename files", "Exporting error", wxICON_ERROR | wxOK | wxCENTRE);
			return;
		}

		if (!DeleteItems({ "Exported.pdb", "Engine.pdb", "Engine.lib", "Engine.exp" })) {
			wxMessageBox("Could not delete unnecesarry files", "Exporting error", wxICON_ERROR | wxOK | wxCENTRE);
			return;
		}

		if (wxExecute("\"" + ProjectData::GetPath() + "Solution\\GenSolution.bat\" -nopause", wxEXEC_SYNC) != 0) {
			wxMessageBox("Could not regenerate the main project", "Exporting error", wxICON_ERROR | wxOK | wxCENTRE);
			return;
		}

		wxMessageBox("The exporting was successful", "Success");
	}


	bool Exporter::CopyFolderContent(const wxString& folder)
	{
		wxString from = ProjectData::GetPath() + folder;
		wxString to = ProjectData::GetPath() + "Exported\\" + folder;

		bool success = true;
		success &= wxFileName::Mkdir(to, wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
		if (success) {
			wxDir dir(from);
			success &= CopyFolderContent(dir, to);
		}
		return success;
	}


	bool Exporter::CopyFolderContent(wxDir& from, const wxString& toUnder)
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


	bool Exporter::CopyItem(const wxString& from, const wxString& to)
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

	bool Exporter::PrecompilePyScripts()
	{
		wxString pyFolder = ProjectData::GetPath() + "Scripts\\";
		wxString pycFolder = ProjectData::GetPath() + "Exported\\Scripts\\";

		bool success = wxFileName::Mkdir(pycFolder, wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
		if (success) {
			wxDir dir(pyFolder);
			success &= PrecompilePyScripts(dir, pycFolder);
		}
		return success;
	}

	bool Exporter::PrecompilePyScripts(wxDir& pyFolder, const wxString& pycFolder)
	{
		bool success = true;

		wxString fileName;
		bool found = pyFolder.GetFirst(&fileName);

		while (found && success) {
			wxString fullPath = pyFolder.GetNameWithSep() + fileName;
			if (wxFileName::DirExists(fullPath)) {
				wxDir subDir(fullPath);
				success &= PrecompilePyScripts(subDir, pycFolder + fileName + "\\");
			} else if (fileName.Length() > 3 && fileName.EndsWith(".py")) {
				if (!wxFileName::DirExists(pyFolder.GetName())) {
					success &= wxFileName::Mkdir(pycFolder, wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
				}
				success &= PythonInterface::Precompile(wxStr2StdStr(fullPath), wxStr2StdStr(pycFolder + fileName + "c"));
			}

			found = pyFolder.GetNext(&fileName);
		}

		return success;
	}

	bool Exporter::DeleteItems(const std::vector<wxString> items)
	{
		for (const wxString& item : items) {
			wxString itemFullPath = ProjectData::GetPath() + "Exported\\" + item;
			if (wxFileExists(itemFullPath) && !wxRemoveFile(itemFullPath)) {
				return false;
			}
		}
		return true;
	}

}
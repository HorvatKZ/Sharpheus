#include "editor_pch.h"
#include <wx/dir.h>
#include <wx/filename.h>
#include "Exporter.hpp"
#include "Editor/Registry/ProjectData.hpp"


namespace Sharpheus {

	Exporter::Exporter()
	{
		Export();
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
		if (!success) {
			wxMessageBox("Could not copy Assets and/or Levels folder", "Exporting error", wxICON_ERROR | wxOK | wxCENTRE);
			return;
		}

		if (wxExecute("\"" + ProjectData::GetPath() + "Solution\\Exported\\GenSolution.bat\"", wxEXEC_SYNC) != 0) {
			wxMessageBox("Could not generate Exported project", "Exporting error", wxICON_ERROR | wxOK | wxCENTRE);
			return;
		}

		if (wxExecute("MSBuild.exe \"" + ProjectData::GetPath() + "Solution\\Exported\\Exported.sln\"", wxEXEC_SYNC) != 0) {
			wxMessageBox("Could not build Exported project", "Exporting error", wxICON_ERROR | wxOK | wxCENTRE);
			return;
		}

		if (!wxRenameFile(ProjectData::GetPath() + "Exported\\Exported.exe",
			ProjectData::GetPath() + "Exported\\" + ProjectData::GetProj()->GetName() + ".exe")) {
			wxMessageBox("Could not rename files", "Exporting error", wxICON_ERROR | wxOK | wxCENTRE);
			return;
		}

		if (!wxRemoveFile(ProjectData::GetPath() + "Exported\\Exported.pdb")) {
			wxMessageBox("Could not delete unnecesarry files", "Exporting error", wxICON_ERROR | wxOK | wxCENTRE);
			return;
		}

		if (!RegenerateMainProject()) {
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

	bool Exporter::RegenerateMainProject()
	{
		bool found;
		wxString name;
		wxDir dir(ProjectData::GetPath() + "Solution");
		found = dir.GetFirst(&name);
		while (found && name.Left(11) != "GenSolution") {
			found = dir.GetNext(&name);
		}

		if (name.Length() < 11 || name.Left(11) != "GenSolution") {
			return false;
		}

		wxExecute(ProjectData::GetPath() + "Solution\\" + name, wxEXEC_SYNC);
		return true;
	}

}
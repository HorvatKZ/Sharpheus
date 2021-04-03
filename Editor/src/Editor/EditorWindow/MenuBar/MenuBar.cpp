#include "editor_pch.h"
#include "MenuBar.hpp"
#include "ProjectSettingsDialog.hpp"
#include "WindowSettingsDialog.hpp"
#include "AnimationCreatorDialog.hpp"
#include "Editor/Registry/EditorData.hpp"
#include "Editor/Registry/ProjectData.hpp"
#include "Editor/FileUtils/RelativeFileDialog.hpp"
#include "Editor/Exporting/Exporter.hpp"


namespace Sharpheus {

	MenuBar::MenuBar(wxWindow* parent) : wxMenuBar(), parent(parent)
	{
		wxMenu* project = new wxMenu;
		project->Append(10001, wxT("Project Settings\tCtrl+Alt+P"));
		project->Append(10002, wxT("Window Settings\tCtrl+Alt+W"));
		Append(project, "Project");

		wxMenu* level = new wxMenu;
		level->Append(10101, wxT("New level\tCtrl+N"));
		level->Append(10102, wxT("Open level\tCtrl+O"));
		level->Append(10103, wxT("Save level\tCtrl+S"));
		level->Append(10104, wxT("Save as..\tCtrl+Shift+S"));
		Append(level, "Level");

		wxMenu* editor = new wxMenu;
		editor->Append(10201, wxT("Editor Settings\tCtrl+Alt+E"));
		editor->Append(10202, wxT("Grid Settings\tCtrl+Alt+G"));
		editor->Append(10203, wxT("Animation Creator"));
		Append(editor, "Editor");

		wxMenu* exporting = new wxMenu;
		exporting->Append(10301, wxT("Export the Game"));
		Append(exporting, "Export");

		Connect(10001, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MenuBar::ProjectSettings));
		Connect(10002, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MenuBar::WindowSettings));
		Connect(10101, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MenuBar::NewLevel));
		Connect(10102, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MenuBar::LoadLevel));
		Connect(10103, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MenuBar::SaveLevel));
		Connect(10104, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MenuBar::SaveLevelAs));
		Connect(10201, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MenuBar::EditorSettings));
		Connect(10202, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MenuBar::GridSettings));
		Connect(10203, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MenuBar::AnimatorCreator));
		Connect(10301, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MenuBar::ExportGame));
	}


	MenuBar::~MenuBar()
	{
	}


	void MenuBar::BindCallbacks(std::function<void()>&& levelChangedCallback)
	{
		this->levelChangedCallback = std::move(levelChangedCallback);
	}


	void MenuBar::ProjectSettings(wxCommandEvent& e)
	{
		ProjectSettingsDialog dialog(parent, ProjectData::GetPath());
		dialog.FillWithData(ProjectData::GetProj()->GetName(), ProjectData::GetProj()->GetDefaultLevelPath());
		
		if (dialog.ShowModal() == wxID_CANCEL) {
			return;
		}

		wxString newProjName = dialog.GetProjectName();
		if (!newProjName.empty()) {
			ProjectData::GetProj()->SetName(wxStr2StdStr(newProjName));
			((wxFrame*)parent)->SetTitle("Sharpheus Editor - " + newProjName);
		}
		ProjectData::GetProj()->SetDefaultLevelPath(wxStr2StdStr(dialog.GetDefaultLevelPath()));

		bool success = ProjectData::Save();
		SPHE_ASSERT(success, "An error occured during the saving of the project settings. Check the logs for more information");
	}


	void MenuBar::WindowSettings(wxCommandEvent& e)
	{
		WindowSettingsDialog dialog(parent);
		dialog.FillWithData(ProjectData::GetWinProps());

		if (dialog.ShowModal() == wxID_CANCEL) {
			return;
		}

		ProjectData::SetWinProps(dialog.GetWinProps());
		bool success = ProjectData::Save();
		SPHE_ASSERT(success, "An error occured during the saving of the project settings. Check the logs for more information");
		parent->Refresh();
	}


	void MenuBar::NewLevel(wxCommandEvent& e)
	{
		int response = wxMessageBox("Do you want to save the current level?", "Save", wxICON_WARNING | wxYES | wxNO | wxCENTER);

		if (response == wxYES) {
			SaveLevel(e);
		}

		wxTextEntryDialog levelNameDialog(this, "Level name:", "New Level");
		levelNameDialog.SetTextValidator(wxFILTER_EMPTY);

		if (levelNameDialog.ShowModal() == wxID_CANCEL)
			return;
		
		ProjectData::CreateNewLevel(wxStr2StdStr(levelNameDialog.GetValue()));
		levelChangedCallback();
	}


	void MenuBar::LoadLevel(wxCommandEvent& e)
	{
		int response = wxMessageBox("Do you want to save the current level?", "Save", wxICON_WARNING | wxYES | wxNO | wxCENTER);

		if (response == wxYES) {
			SaveLevel(e);
		}

		RelativeOpenDialog openDialog(this, "Open Level", ProjectData::GetPath() + "Levels\\", "Sharpheus level file(*.lvl.sharpheus) | *.lvl.sharpheus");

		if (!openDialog.Show())
			return;

		bool success = ProjectData::GetProj()->LoadLevel(wxStr2StdStr(openDialog.GetPath()));
		SPHE_ASSERT(success, "Cannot load level. Check the log files for more information");
		levelChangedCallback();
	}


	void MenuBar::SaveLevel(wxCommandEvent& e)
	{
		if (ProjectData::GetLevel()->HasPath()) {
			bool success = ProjectData::GetLevel()->Save();
			SPHE_ASSERT(success, "Cannot save level. Check the log files for more information");
		}
		else {
			SaveLevelAs(e);
		}
	}


	void MenuBar::SaveLevelAs(wxCommandEvent& /*e*/)
	{
		RelativeSaveDialog saveDialog(this, "Save Level", ProjectData::GetPath() + "Levels\\", "Sharpheus level file(*.lvl.sharpheus) | *.lvl.sharpheus");

		if (!saveDialog.Show())
			return;

		bool success = ProjectData::GetProj()->SaveLevel(wxStr2StdStr(saveDialog.GetPath()));
		SPHE_ASSERT(success, "Cannot save level. Check the log files for more information");
	}


	void MenuBar::EditorSettings(wxCommandEvent& e)
	{
		wxMessageBox("Editor settings is under construction..", "Info");
	}


	void MenuBar::GridSettings(wxCommandEvent& e)
	{
		wxMessageBox("Grid settings is under construction..", "Info");
	}


	void MenuBar::AnimatorCreator(wxCommandEvent& e)
	{
		AnimationCreatorDialog creator(parent);

		if (creator.ShowModal() != wxID_CANCEL)
			return;
	}


	void MenuBar::ExportGame(wxCommandEvent& e)
	{
		int response = wxMessageBox("Do you want to save the current level before exporting?", "Save", wxICON_WARNING | wxYES | wxNO | wxCENTER);

		if (response == wxYES) {
			SaveLevel(e);
		}

		Exporter exp;
	}

}
#include "editor_pch.h"
#include "MenuBar.hpp"
#include "Editor/EditorCommands.hpp"
#include "ProjectSettingsDialog.hpp"
#include "WindowSettingsDialog.hpp"
#include "GridSettingsDialog.hpp"
#include "AnimationCreatorDialog.hpp"
#include "AboutDialog.hpp"
#include "Editor/Registry/EditorData.hpp"
#include "Editor/Registry/ProjectData.hpp"
#include "Editor/FileUtils/RelativeFileDialog.hpp"
#include "Editor/Exporting/Exporter.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"


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
		level->AppendSeparator();
		level->Append(10105, wxT("Attach Scene"));
		level->Append(10106, wxT("Save as Scene"));
		Append(level, "Level");

		wxMenu* editor = new wxMenu;
		editor->Append(10201, wxT("Grid Settings\tCtrl+Alt+G"));
		editor->AppendSeparator();
		editor->Append(10202, wxT("Animation Creator"));
		Append(editor, "Editor");

		wxMenu* exporting = new wxMenu;
		exporting->Append(10301, wxT("Export with logging"));
		exporting->Append(10302, wxT("Export final verison"));
		Append(exporting, "Export");

		wxMenu* about = new wxMenu;
		about->Append(10401, wxT("About Sharpheus"));
		Append(about, "About");

		Connect(10001, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MenuBar::ProjectSettings));
		Connect(10002, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MenuBar::WindowSettings));
		Connect(10101, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MenuBar::NewLevel));
		Connect(10102, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MenuBar::LoadLevel));
		Connect(10103, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MenuBar::SaveLevel));
		Connect(10104, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MenuBar::SaveLevelAs));
		Connect(10105, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MenuBar::AttachScene));
		Connect(10106, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MenuBar::SaveAsScene));
		Connect(10201, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MenuBar::GridSettings));
		Connect(10202, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MenuBar::AnimatorCreator));
		Connect(10301, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MenuBar::ExportWithLogs));
		Connect(10302, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MenuBar::ExportFinal));
		Connect(10401, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MenuBar::About));
	}


	MenuBar::~MenuBar()
	{
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
		EditorCommands::NewLevel();
	}


	void MenuBar::LoadLevel(wxCommandEvent& e)
	{
		EditorCommands::LoadLevel();
	}


	void MenuBar::SaveLevel(wxCommandEvent& e)
	{
		EditorCommands::SaveLevel();
	}


	void MenuBar::SaveLevelAs(wxCommandEvent& e)
	{
		EditorCommands::SaveLevelAs();
	}


	void MenuBar::AttachScene(wxCommandEvent& e)
	{
		EditorCommands::AttachSceneToCurrent();
	}


	void MenuBar::SaveAsScene(wxCommandEvent& e)
	{
		EditorCommands::SaveCurrentAsScene();
	}


	void MenuBar::GridSettings(wxCommandEvent& e)
	{
		GridSettingsDialog dialog(parent);
		dialog.FillWithData(EditorData::GetGridProps());

		if (dialog.ShowModal() == wxID_CANCEL) {
			return;
		}

		EditorData::SetGridProps(dialog.GetGridProps());
		parent->Refresh();
	}


	void MenuBar::AnimatorCreator(wxCommandEvent& e)
	{
		EditorCommands::CreateAnimation();
	}


	void MenuBar::ExportWithLogs(wxCommandEvent& e)
	{
		EditorCommands::ExportTheGame(true);
	}


	void MenuBar::ExportFinal(wxCommandEvent& e)
	{
		EditorCommands::ExportTheGame(false);
	}


	void MenuBar::About(wxCommandEvent& e)
	{
		AboutDialog about(parent);

		about.ShowModal();
	}

}
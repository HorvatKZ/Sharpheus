#include "starter_pch.h"
#include "StarterWindow.hpp"
#include "ProcessControl.hpp"
#include "NewProjectDialog.hpp"
#include <wx/stdpaths.h>



namespace Sharpheus {

	uint32_t StarterWindow::border = 5;
	wxSize StarterWindow::buttonSize(200, 30);


	StarterWindow::StarterWindow(const std::string& title, uint32_t width, uint32_t height)
		: wxFrame(nullptr, wxID_ANY, title, wxPoint(100, 100), wxSize(width, height))
	{
		SetMinSize(GetSize());
		SetMaxSize(GetSize());

		wxIcon icon;
		icon.CopyFromBitmap(wxBitmap(ProcessControl::GetRoot() + "Assets\\Editor\\Icons\\sharpheus_icon.png", wxBITMAP_TYPE_PNG));
		SetIcon(icon);

		header = new wxStaticBitmap(this, wxID_ANY, wxBitmap(ProcessControl::GetRoot() + "Assets\\Branding\\sharpheus_long_logo.png", wxBITMAP_TYPE_PNG), wxPoint(0, 0), wxSize(1000, 200));
		
		createButton = new wxButton(this, wxID_ANY, "Create new project", wxPoint(border, 200 + border), buttonSize);
		createButton->Bind(wxEVT_BUTTON, &StarterWindow::OnNew, this);
		browseButton = new wxButton(this, wxID_ANY, "Browse for project", wxPoint(3 * border + buttonSize.x, 200 + border), buttonSize);
		browseButton->Bind(wxEVT_BUTTON, &StarterWindow::OnBrowse, this);

		wxSize realSize = GetClientSize();
		searchBox = new wxSearchCtrl(this, wxID_ANY, "", wxPoint(realSize.x - border - 300, 200 + border), wxSize(300, buttonSize.y), wxTE_PROCESS_ENTER);
		searchBox->ShowSearchButton(true);
		searchBox->ShowCancelButton(true);
		searchBox->Bind(wxEVT_TEXT_ENTER, &StarterWindow::OnSearch, this);
		searchBox->Bind(wxEVT_SEARCHCTRL_SEARCH_BTN, &StarterWindow::OnSearch, this);
		searchBox->Bind(wxEVT_SEARCHCTRL_CANCEL_BTN, &StarterWindow::OnExitSearch, this);

		projectList = new ProjectList(this, wxPoint(0, 200 + 2 * border + buttonSize.y), wxSize(realSize.x, realSize.y - 200 - 2 * border - buttonSize.y),
			wxStandardPaths::Get().GetUserConfigDir() + "\\Sharpheus\\projects.txt");
	}


	StarterWindow::~StarterWindow()
	{
	}


	void StarterWindow::OnNew(wxCommandEvent& e)
	{
		NewProjectDialog dialog(this, wxStandardPaths::Get().GetDocumentsDir());

		if (dialog.ShowModal() == wxID_CANCEL) {
			return;
		}

		ProcessControl::CreateNewProject(dialog.GetProjectName(), dialog.GetLevelName(), dialog.GetFolderPath(), dialog.GetIDE(), dialog.HasToBuild());
	}


	void StarterWindow::OnBrowse(wxCommandEvent& e)
	{
		wxFileDialog dialog(this, "Browse for project", "", "", "Sharpheus project(*.proj.sharpheus) | *.proj.sharpheus");

		if (dialog.ShowModal() == wxID_CANCEL) {
			return;
		}

		wxString project = dialog.GetPath();
		projectList->AppendProject(project);
		ProcessControl::OpenProj(project);
	}


	void StarterWindow::OnSearch(wxCommandEvent& e)
	{
		projectList->Search(searchBox->GetValue());
	}


	void StarterWindow::OnExitSearch(wxCommandEvent& e)
	{
		searchBox->SetValue("");
		projectList->ExitSearch();
	}

}
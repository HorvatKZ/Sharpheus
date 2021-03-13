#include "editor_pch.h"
#include "ProjectSettingsDialog.hpp"
#include "Editor/FileUtils/RelativeFileDialog.hpp"


namespace Sharpheus {

	ProjectSettingsDialog::ProjectSettingsDialog(wxWindow* parent, const wxString& projectFolder)
		: SettingsDialog(parent, "Project Setting", wxSize(400, 180)), projectFolder(projectFolder)
	{
		uint32_t realWidth = GetClientSize().x;
		nameLabel = new wxStaticText(this, wxID_ANY, "Project name:", wxPoint(UI::border, UI::border + 3), wxSize(120, 22));
		nameLabel->SetFont(UI::titleFont);

		nameInput = new wxTextCtrl(this, wxID_ANY, "", wxPoint(2 * UI::border + 120, UI::border), wxSize(realWidth - 120 - 3 * UI::border, 22), wxTE_PROCESS_ENTER);
		nameInput->SetValidator(wxTextValidator(wxFILTER_EMPTY));

		defaultLevelPathLabel = new wxStaticText(this, wxID_ANY, "Default level:", wxPoint(UI::border, 2 * UI::border + 30), wxSize(100, 22));
		defaultLevelPathLabel->SetFont(UI::titleFont);

		wxSize pathTextSize(realWidth - 3 * UI::border - UI::buttonSize.x, UI::buttonSize.y);
		pathText = new wxStaticText(this, wxID_ANY, "", wxPoint(UI::border, 2 * UI::border + 52), pathTextSize, wxST_ELLIPSIZE_START);
		pathText->SetMaxSize(pathTextSize);

		browseButton = new wxButton(this, wxID_ANY, "Browse...", wxPoint(realWidth - UI::border - UI::buttonSize.x , 2 * UI::border + 52), UI::buttonSize);
		browseButton->Bind(wxEVT_BUTTON, &ProjectSettingsDialog::BrowseClicked, this);
	}


	ProjectSettingsDialog::~ProjectSettingsDialog()
	{
	}


	void ProjectSettingsDialog::FillWithData(const wxString& projectName, const wxString& defaultLevelPath)
	{
		nameInput->SetValue(projectName);
		pathText->SetLabel(defaultLevelPath);
	}


	void ProjectSettingsDialog::EndModal(int retCode)
	{
		projectName = nameInput->GetValue();
		wxDialog::EndModal(retCode);
	}


	void ProjectSettingsDialog::BrowseClicked(wxCommandEvent& e)
	{
		RelativeOpenDialog openDialog(this, "Open Level", projectFolder + "Levels\\", "Sharpheus level file(*.lvl.sharpheus) | *.lvl.sharpheus");

		if (!openDialog.Show())
			return;

		defaultLevelPath = openDialog.GetPath();
		pathText->SetLabel(defaultLevelPath);
	}

}
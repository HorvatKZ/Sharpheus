#include "editor_pch.h"
#include "ProjectSettingsDialog.hpp"
#include "Editor/FileUtils/RelativeFileDialog.hpp"


namespace Sharpheus {

	uint32_t ProjectSettingsDialog::labelWidth = 120;


	ProjectSettingsDialog::ProjectSettingsDialog(wxWindow* parent, const wxString& projectFolder)
		: SettingsDialog(parent, "Project Setting", wxSize(400, 180)), projectFolder(projectFolder)
	{
		uint32_t realWidth = GetClientSize().x;
		uint32_t y = UI::border;
		nameLabel = new wxStaticText(this, wxID_ANY, "Project name:", wxPoint(UI::border, y + UI::shift), wxSize(labelWidth, UI::unitHeight));
		nameLabel->SetFont(UI::titleFont);

		nameInput = new wxTextCtrl(this, wxID_ANY, "", wxPoint(2 * UI::border + labelWidth, y), wxSize(realWidth - labelWidth - 3 * UI::border, UI::unitHeight), wxTE_PROCESS_ENTER);
		nameInput->SetValidator(wxTextValidator(wxFILTER_EMPTY));

		y += UI::border + UI::heightPadding;
		defaultLevelPathLabel = new wxStaticText(this, wxID_ANY, "Default level:", wxPoint(UI::border, y), wxSize(labelWidth, UI::unitHeight));
		defaultLevelPathLabel->SetFont(UI::titleFont);

		y += UI::unitHeight;
		wxSize pathTextSize(realWidth - 3 * UI::border - UI::buttonSize.x, UI::buttonSize.y);
		pathText = new wxStaticText(this, wxID_ANY, "", wxPoint(UI::border, y), pathTextSize, wxST_ELLIPSIZE_START);
		pathText->SetMaxSize(pathTextSize);

		browseButton = new wxButton(this, wxID_ANY, "Browse...", wxPoint(realWidth - UI::border - UI::buttonSize.x , y), UI::buttonSize);
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
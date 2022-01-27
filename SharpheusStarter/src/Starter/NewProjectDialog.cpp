#include "starter_pch.h"
#include "NewProjectDialog.hpp"


namespace Sharpheus {

	uint32_t NewProjectDialog::border = 5;
	wxSize NewProjectDialog::buttonSize(90, 26);
	wxFont NewProjectDialog::titleFont(10, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	std::map<wxString, wxString, std::greater<wxString>> NewProjectDialog::ides({
		{"Visual Studio 2017", "vs2017"},
		{"Visual Studio 2019", "vs2019"},
		{"Visual Studio 2022", "vs2022"}
	});


	NewProjectDialog::NewProjectDialog(wxWindow* parent, const wxString& folderPath)
		: wxDialog(parent, wxID_ANY, "Create new project", wxPoint(0, 0), wxSize(400, 250))
	{
		wxSize parentSize = parent->GetSize();
		SetPosition(parent->GetPosition() + wxPoint((parentSize.x - 400) / 2, (parentSize.y - 300) / 2));

		uint32_t realWidth = GetClientSize().x;
		projectNameLabel = new wxStaticText(this, wxID_ANY, "Name:", wxPoint(border, border + 3), wxSize(120, 22));
		projectNameLabel->SetFont(titleFont);

		projectNameInput = new wxTextCtrl(this, wxID_ANY, "", wxPoint(2 * border + 120, border), wxSize(realWidth - 120 - 3 * border, 22));
		projectNameInput->SetValidator(wxTextValidator(wxFILTER_EMPTY));

		levelNameLabel = new wxStaticText(this, wxID_ANY, "Default level:", wxPoint(border, 2 * border + 28), wxSize(120, 22));
		levelNameLabel->SetFont(titleFont);

		levelNameInput = new wxTextCtrl(this, wxID_ANY, "", wxPoint(2 * border + 120, 2 * border + 25), wxSize(realWidth - 120 - 3 * border, 22));
		levelNameInput->SetValidator(wxTextValidator(wxFILTER_EMPTY));

		IDELabel = new wxStaticText(this, wxID_ANY, "IDE:", wxPoint(border, 3 * border + 53), wxSize(120, 22));
		IDELabel->SetFont(titleFont);

		wxArrayString idesArr;
		for (auto it = ides.begin(); it != ides.end(); ++it) {
			idesArr.Add((*it).first);
		}
		IDEPicker = new wxComboBox(this, wxID_ANY, idesArr[0], wxPoint(2 * border + 120, 3 * border + 50), wxSize(realWidth - 120 - 3 * border, 22), idesArr);
		IDEPicker->SetEditable(false);

		folderNameLabel = new wxStaticText(this, wxID_ANY, "Base folder:", wxPoint(border, 4 * border + 78), wxSize(60, 22));
		folderNameLabel->SetFont(titleFont);

		wxSize pathTextSize(realWidth - 3 * border - buttonSize.x, buttonSize.y);
		folderPathText = new wxStaticText(this, wxID_ANY, folderPath, wxPoint(border, 4 * border + 100), pathTextSize, wxST_ELLIPSIZE_START);
		folderPathText->SetMaxSize(pathTextSize);

		browseButton = new wxButton(this, wxID_ANY, "Choose...", wxPoint(realWidth - border - buttonSize.x, 4 * border + 98), buttonSize);
		browseButton->Bind(wxEVT_BUTTON, &NewProjectDialog::OnBrowse, this);

		buildLabel = new wxStaticText(this, wxID_ANY, "Build now", wxPoint(border, 5 * border + 125), wxSize(120, 22));
		buildLabel->SetFont(titleFont);

		buildCheckBox = new wxCheckBox(this, wxID_ANY, "", wxPoint(2 * border + 120, 5 * border + 122), wxSize(22, 22));
		buildCheckBox->SetValue(true);

		wxSize clientSize = GetClientSize();
		createButton = new wxButton(this, wxID_OK, "Create", wxPoint(clientSize.x - 2 * border - 2 * buttonSize.x, clientSize.y - border - buttonSize.y), buttonSize);
		cancelButton = new wxButton(this, wxID_CANCEL, "Cancel", wxPoint(clientSize.x - border - buttonSize.x, clientSize.y - border - buttonSize.y), buttonSize);
	}


	NewProjectDialog::~NewProjectDialog()
	{
	}


	void NewProjectDialog::EndModal(int retCode)
	{
		projectName = projectNameInput->GetValue();
		levelName = levelNameInput->GetValue();
		folderPath = folderPathText->GetLabel();
		ide = ides[IDEPicker->GetValue()];
		build = buildCheckBox->GetValue();

		wxDialog::EndModal(retCode);
	}


	void NewProjectDialog::OnBrowse(wxCommandEvent& e)
	{
		wxDirDialog dialog(this, "Choose base folder", folderPathText->GetLabel(), wxDD_DIR_MUST_EXIST);

		if (dialog.ShowModal() == wxID_CANCEL) {
			return;
		}

		folderPathText->SetLabel(dialog.GetPath());
	}

}
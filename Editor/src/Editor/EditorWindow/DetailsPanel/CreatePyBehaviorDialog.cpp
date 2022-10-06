#include "editor_pch.h"
#include "CreatePyBehaviorDialog.hpp"
#include "BehaviorCreator.hpp"
#include "Editor/FileUtils/RelativeFileDialog.hpp"
#include <wx/valnum.h>


namespace Sharpheus {

	const uint32 CreatePyBehaviorDialog::labelWidth = 60;


	CreatePyBehaviorDialog::CreatePyBehaviorDialog(wxWindow* parent, const wxString& projectFolder)
		: wxDialog(parent, wxID_ANY, "Create Behavior", wxPoint(0, 0), wxSize(400, 170)), projectFolder(projectFolder)
	{
		wxSize parentSize = parent->GetSize();
		wxSize size = GetClientSize();
		SetPosition(wxPoint((parentSize.x - size.x) / 2, (parentSize.y - size.y) / 2));

		uint32 realWidth = GetClientSize().x;
		uint32 y = UI::border;
		classNameLabel = new wxStaticText(this, wxID_ANY, "Name:", wxPoint(UI::border, y + UI::shift), wxSize(labelWidth, UI::unitHeight));
		classNameLabel->SetFont(UI::titleFont);

		classNameInput = new wxTextCtrl(this, wxID_ANY, "", wxPoint(2 * UI::border + labelWidth, y), wxSize(realWidth - labelWidth - 3 * UI::border, UI::unitHeight));
		classNameInput->SetValidator(wxTextValidator(wxFILTER_EMPTY | wxFILTER_ALPHA));

		y += UI::border + UI::unitHeight + UI::shift;
		fileLabel = new wxStaticText(this, wxID_ANY, "File:", wxPoint(UI::border, y + UI::shift), wxSize(labelWidth, UI::unitHeight));
		fileLabel->SetFont(UI::titleFont);

		y += UI::unitHeight + UI::shift;
		wxSize pathTextSize(realWidth - 3 * UI::border - UI::buttonSize.x, UI::buttonSize.y);
		pathText = new wxStaticText(this, wxID_ANY, "...", wxPoint(UI::border, y + UI::shift), pathTextSize, wxST_ELLIPSIZE_START);
		pathText->SetMaxSize(pathTextSize);

		browseButton = new wxButton(this, wxID_ANY, "Choose...", wxPoint(realWidth - UI::border - UI::buttonSize.x, y), UI::buttonSize);
		browseButton->Bind(wxEVT_BUTTON, &CreatePyBehaviorDialog::OnBrowse, this);

		wxSize clientSize = GetClientSize();
		createButton = new wxButton(this, wxID_OK, "Create", wxPoint(clientSize.x - 2 * UI::border - 2 * UI::buttonSize.x, clientSize.y - UI::border - UI::buttonSize.y), UI::buttonSize);
		cancelButton = new wxButton(this, wxID_CANCEL, "Cancel", wxPoint(clientSize.x - UI::border - UI::buttonSize.x, clientSize.y - UI::border - UI::buttonSize.y), UI::buttonSize);
	}


	CreatePyBehaviorDialog::~CreatePyBehaviorDialog()
	{
	}


	void CreatePyBehaviorDialog::EndModal(int retCode)
	{
		if (retCode == wxID_OK && !pathFilled) {
			SPHE_WARN("Please choose a location for the Python script first.");
			return;
		}

		className = classNameInput->GetValue();
		fileName = pathText->GetLabel();

		wxDialog::EndModal(retCode);
	}


	void CreatePyBehaviorDialog::OnBrowse(wxCommandEvent& e)
	{
		RelativeSaveDialog dialog(this, "Choose script location", projectFolder + "Scripts\\", "Python file(*.py) | *.py");

		if (!dialog.Show())
			return;

		pathText->SetLabel("Scripts\\" + dialog.GetPath());
		pathFilled = true;
	}

}
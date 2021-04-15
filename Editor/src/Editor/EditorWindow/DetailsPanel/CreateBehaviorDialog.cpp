#include "editor_pch.h"
#include "CreateBehaviorDialog.hpp"
#include "BehaviorCreator.hpp"
#include "Editor/FileUtils/RelativeDirDialog.hpp"
#include <wx/valnum.h>


namespace Sharpheus {

	CreateBehaviorDialog::CreateBehaviorDialog(wxWindow* parent, const wxString& projectFolder)
		: wxDialog(parent, wxID_ANY, "Create Behavior", wxPoint(0, 0), wxSize(400, 230)), projectFolder(projectFolder)
	{
		wxSize parentSize = parent->GetSize();
		SetPosition(wxPoint((parentSize.x - 400) / 2, (parentSize.y - 300) / 2));

		uint32_t realWidth = GetClientSize().x;
		classNameLabel = new wxStaticText(this, wxID_ANY, "Name:", wxPoint(UI::border, UI::border + 3), wxSize(60, 22));
		classNameLabel->SetFont(UI::titleFont);

		classNameInput = new wxTextCtrl(this, wxID_ANY, "", wxPoint(2 * UI::border + 60, UI::border), wxSize(realWidth - 60 - 3 * UI::border, 22));
		classNameInput->SetValidator(wxTextValidator(wxFILTER_EMPTY | wxFILTER_ALPHA));

		parentClassLabel = new wxStaticText(this, wxID_ANY, "Parent class:", wxPoint(UI::border, 2 * UI::border + 28), wxSize(120, 22));
		parentClassLabel->SetFont(UI::titleFont);

		uint32_t maxId = 0;
		wxArrayString parentClasses;
		parentClasses.Add("Sharpheus::Behavior");
		for (auto it = BehaviorCreator::behaviorNames.begin(); it != BehaviorCreator::behaviorNames.end(); ++it) {
			parentClasses.Add((*it).second);
			
			if ((*it).first > maxId) {
				maxId = (*it).first;
			}
		}
		parentClassPicker = new wxComboBox(this, wxID_ANY, parentClasses[0], wxPoint(2 * UI::border + 120, 2 * UI::border + 25), wxSize(realWidth - 120 - 3 * UI::border, 22), parentClasses);
		parentClassPicker->SetEditable(false);
			
		idLabel = new wxStaticText(this, wxID_ANY, "ID:", wxPoint(UI::border, 3 * UI::border + 53), wxSize(60, 22));
		idLabel->SetFont(UI::titleFont);

		idInput = new wxTextCtrl(this, wxID_ANY, wxString::Format("%d", maxId + 1), wxPoint(2 * UI::border + 60, 3 * UI::border + 50), wxSize(60, 22));
		idInput->SetValidator(wxIntegerValidator<uint32_t>());

		baseFolderLabel = new wxStaticText(this, wxID_ANY, "Folder:", wxPoint(UI::border, 4 * UI::border + 78), wxSize(60, 22));
		baseFolderLabel->SetFont(UI::titleFont);

		wxSize pathTextSize(realWidth - 3 * UI::border - UI::buttonSize.x, UI::buttonSize.y);
		pathText = new wxStaticText(this, wxID_ANY, "Source\\CustomBehaviors\\", wxPoint(UI::border, 4 * UI::border + 103), pathTextSize, wxST_ELLIPSIZE_START);
		pathText->SetMaxSize(pathTextSize);

		browseButton = new wxButton(this, wxID_ANY, "Choose...", wxPoint(realWidth - UI::border - UI::buttonSize.x, 4 * UI::border + 100), UI::buttonSize);
		browseButton->Bind(wxEVT_BUTTON, &CreateBehaviorDialog::OnBrowse, this);

		wxSize clientSize = GetClientSize();
		createButton = new wxButton(this, wxID_OK, "Create", wxPoint(clientSize.x - 2 * UI::border - 2 * UI::buttonSize.x, clientSize.y - UI::border - UI::buttonSize.y), UI::buttonSize);
		cancelButton = new wxButton(this, wxID_CANCEL, "Cancel", wxPoint(clientSize.x - UI::border - UI::buttonSize.x, clientSize.y - UI::border - UI::buttonSize.y), UI::buttonSize);
	}


	CreateBehaviorDialog::~CreateBehaviorDialog()
	{
	}


	void CreateBehaviorDialog::EndModal(int retCode)
	{
		className = classNameInput->GetValue();
		parentClassName = parentClassPicker->GetValue();
		folder = pathText->GetLabel();

		unsigned long val;
		idInput->GetValue().ToULong(&val);
		id = val;

		wxDialog::EndModal(retCode);
	}


	void CreateBehaviorDialog::OnBrowse(wxCommandEvent& e)
	{
		RelativeDirDialog dialog(this, "Choose base folder", projectFolder + "Source\\CustomBehaviors");

		if (!dialog.Show())
			return;

		pathText->SetLabel("Source\\CustomBehaviors\\" + dialog.GetPath());
	}

}
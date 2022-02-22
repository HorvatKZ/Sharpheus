#include "editor_pch.h"
#include "CreateBehaviorDialog.hpp"
#include "BehaviorCreator.hpp"
#include "Editor/FileUtils/RelativeDirDialog.hpp"
#include <wx/valnum.h>


namespace Sharpheus {

	const uint32 CreateBehaviorDialog::labelWidth = 60;


	CreateBehaviorDialog::CreateBehaviorDialog(wxWindow* parent, const wxString& projectFolder)
		: wxDialog(parent, wxID_ANY, "Create Behavior", wxPoint(0, 0), wxSize(400, 230)), projectFolder(projectFolder)
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
		parentClassLabel = new wxStaticText(this, wxID_ANY, "Parent class:", wxPoint(UI::border, y + UI::shift), wxSize(2 * labelWidth, UI::unitHeight));
		parentClassLabel->SetFont(UI::titleFont);

		uint32 maxId = 0;
		wxArrayString parentClasses;
		parentClasses.Add("Sharpheus::Behavior");
		for (auto it = ::BehaviorCreator::behaviorNames.begin(); it != ::BehaviorCreator::behaviorNames.end(); ++it) {
			parentClasses.Add((*it).second);
			
			if ((*it).first > maxId) {
				maxId = (*it).first;
			}
		}
		parentClassPicker = new wxComboBox(this, wxID_ANY, parentClasses[0], wxPoint(2 * UI::border + 2 * labelWidth, y), wxSize(realWidth - 2 * labelWidth - 3 * UI::border, UI::unitHeight), parentClasses);
		parentClassPicker->SetEditable(false);
			
		y += UI::border + UI::unitHeight + UI::shift;
		idLabel = new wxStaticText(this, wxID_ANY, "ID:", wxPoint(UI::border, y + UI::shift), wxSize(labelWidth, UI::unitHeight));
		idLabel->SetFont(UI::titleFont);

		idInput = new wxTextCtrl(this, wxID_ANY, wxString::Format("%d", maxId + 1), wxPoint(2 * UI::border + labelWidth, y), wxSize(labelWidth, UI::unitHeight));
		idInput->SetValidator(wxIntegerValidator<uint32>());

		y += UI::border + UI::unitHeight + UI::shift;
		baseFolderLabel = new wxStaticText(this, wxID_ANY, "Folder:", wxPoint(UI::border, y + UI::shift), wxSize(labelWidth, UI::unitHeight));
		baseFolderLabel->SetFont(UI::titleFont);

		y += UI::unitHeight + UI::shift;
		wxSize pathTextSize(realWidth - 3 * UI::border - UI::buttonSize.x, UI::buttonSize.y);
		pathText = new wxStaticText(this, wxID_ANY, "Source\\CustomBehaviors\\", wxPoint(UI::border, y + UI::shift), pathTextSize, wxST_ELLIPSIZE_START);
		pathText->SetMaxSize(pathTextSize);

		browseButton = new wxButton(this, wxID_ANY, "Choose...", wxPoint(realWidth - UI::border - UI::buttonSize.x, y), UI::buttonSize);
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
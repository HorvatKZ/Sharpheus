#include "editor_pch.h"
#include "SettingsDialog.hpp"


namespace Sharpheus {


	SettingsDialog::SettingsDialog(wxWindow* parent, const wxString& title, const wxSize& size)
		: wxDialog(parent, wxID_ANY, title, wxPoint(0, 0), size)
	{
		wxSize parentSize = parent->GetSize();
		SetPosition(wxPoint((parentSize.x - size.x) / 2, (parentSize.y - size.y) / 2));

		wxSize clientSize = GetClientSize();
		okButton = new wxButton(this, wxID_OK, "OK", wxPoint(clientSize.x - 2 * UI::border - 2 * UI::buttonSize.x, clientSize.y - UI::border - UI::buttonSize.y), UI::buttonSize);
		cancelButton = new wxButton(this, wxID_CANCEL, "Cancel", wxPoint(clientSize.x - UI::border - UI::buttonSize.x, clientSize.y - UI::border - UI::buttonSize.y), UI::buttonSize);
	}


	SettingsDialog::~SettingsDialog()
	{
	}

}
#include "editor_pch.h"
#include "SettingsDialog.hpp"


namespace Sharpheus {

	uint32_t SettingsDialog::border = 4;
	wxSize SettingsDialog::buttonSize(90, 26);
	wxFont SettingsDialog::titleFont(10, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);


	SettingsDialog::SettingsDialog(wxWindow* parent, const wxString& title, const wxSize& size)
		: wxDialog(parent, wxID_ANY, title, wxPoint(0, 0), size)
	{
		wxSize parentSize = parent->GetSize();
		SetPosition(wxPoint((parentSize.x - size.x) / 2, (parentSize.y - size.y) / 2));

		wxSize clientSize = GetClientSize();
		okButton = new wxButton(this, wxID_OK, "OK", wxPoint(clientSize.x - 2 * border - 2 * buttonSize.x, clientSize.y - border - buttonSize.y), buttonSize);
		cancelButton = new wxButton(this, wxID_CANCEL, "Cancel", wxPoint(clientSize.x - border - buttonSize.x, clientSize.y - border - buttonSize.y), buttonSize);
	}


	SettingsDialog::~SettingsDialog()
	{
	}

}
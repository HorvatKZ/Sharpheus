#pragma once

#include "editor_pch.h"


namespace Sharpheus {

	class SettingsDialog : public wxDialog
	{
	public:
		SettingsDialog(wxWindow* parent, const wxString& title, const wxSize& size);
		virtual ~SettingsDialog();

	protected:
		wxButton* okButton;
		wxButton* cancelButton;
	};

}
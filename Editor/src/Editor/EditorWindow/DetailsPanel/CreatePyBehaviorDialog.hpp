#pragma once

#include "editor_pch.h"


namespace Sharpheus {

	class CreatePyBehaviorDialog : public wxDialog
	{
	public:
		CreatePyBehaviorDialog(wxWindow* parent, const wxString& projectFolder);
		virtual ~CreatePyBehaviorDialog();

		inline const wxString& GetName() { return className; }
		inline const wxString& GetFile() { return fileName; }

		virtual void EndModal(int retCode) override;

	private:
		wxStaticText* classNameLabel;
		wxStaticText* fileLabel;
		wxStaticText* pathText;
		wxTextCtrl* classNameInput;
		wxButton* browseButton;
		wxButton* createButton;
		wxButton* cancelButton;

		wxString className, fileName, projectFolder;
		bool pathFilled = false;

		static const uint32 labelWidth;

		void OnBrowse(wxCommandEvent& e);
	};

}
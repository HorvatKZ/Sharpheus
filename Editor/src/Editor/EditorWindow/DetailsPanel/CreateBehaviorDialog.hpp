#pragma once

#include "editor_pch.h"


namespace Sharpheus {

	class CreateBehaviorDialog : public wxDialog
	{
	public:
		CreateBehaviorDialog(wxWindow* parent, const wxString& projectFolder);
		virtual ~CreateBehaviorDialog();

		inline const wxString& GetName() { return className; }
		inline const wxString& GetParentName() { return parentClassName; }
		inline const wxString& GetFolder() { return folder; }
		inline uint32_t GetID() { return id; }

		virtual void EndModal(int retCode) override;

	private:
		wxStaticText* classNameLabel;
		wxStaticText* parentClassLabel;
		wxStaticText* idLabel;
		wxStaticText* baseFolderLabel;
		wxStaticText* pathText;
		wxTextCtrl* classNameInput;
		wxTextCtrl* idInput;
		wxComboBox* parentClassPicker;
		wxButton* browseButton;
		wxButton* createButton;
		wxButton* cancelButton;

		wxString className, parentClassName, folder, projectFolder;
		uint32_t id;

		static uint32_t labelWidth;

		void OnBrowse(wxCommandEvent& e);
	};

}
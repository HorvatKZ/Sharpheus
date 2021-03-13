#pragma once

#include "starter_pch.h"


namespace Sharpheus {

	class NewProjectDialog : public wxDialog
	{
	public:
		NewProjectDialog(wxWindow* parent, const wxString& folderPath);
		virtual ~NewProjectDialog();

		inline const wxString& GetProjectName() { return projectName; }
		inline const wxString& GetLevelName() { return levelName; }
		inline const wxString& GetFolderPath() { return folderPath; }
		inline const wxString& GetIDE() { return ide; }
		inline bool HasToBuild() { return build; }

		virtual void EndModal(int retCode) override;

	private:
		wxStaticText* projectNameLabel;
		wxStaticText* levelNameLabel;
		wxStaticText* folderNameLabel;
		wxStaticText* IDELabel;
		wxStaticText* buildLabel;
		wxStaticText* folderPathText;
		wxTextCtrl* projectNameInput;
		wxTextCtrl* levelNameInput;
		wxComboBox* IDEPicker;
		wxCheckBox* buildCheckBox;
		wxButton* browseButton;
		wxButton* createButton;
		wxButton* cancelButton;

		bool build;
		wxString projectName, levelName, folderPath, ide;
		static std::unordered_map<wxString, wxString> ides;
		static uint32_t border;
		static wxSize buttonSize;
		static wxFont titleFont;

		void OnBrowse(wxCommandEvent& e);
	};

}
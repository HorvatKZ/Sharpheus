#pragma once

#include "SettingsDialog.hpp"


namespace Sharpheus {

	class ProjectSettingsDialog : public SettingsDialog
	{
	public:
		ProjectSettingsDialog(wxWindow* parent, const wxString& projectFolder);
		virtual ~ProjectSettingsDialog();

		void FillWithData(const wxString& projectName, const wxString& defaultLevelPath);

		inline const wxString& GetProjectName() { return projectName; }
		inline const wxString& GetDefaultLevelPath() { return defaultLevelPath; }

		virtual void EndModal(int retCode) override;

	private:
		wxStaticText* nameLabel;
		wxTextCtrl* nameInput;
		wxStaticText* defaultLevelPathLabel;
		wxStaticText* pathText;
		wxButton* browseButton;

		wxString projectName, defaultLevelPath, projectFolder;

		static uint32 labelWidth;

		void BrowseClicked(wxCommandEvent& e);

	};

}
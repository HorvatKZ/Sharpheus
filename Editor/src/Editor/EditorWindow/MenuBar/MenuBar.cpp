#include "editor_pch.h"
#include "MenuBar.hpp"
#include "Editor/Registry/EditorData.hpp"


namespace Sharpheus {

	MenuBar::MenuBar() : wxMenuBar()
	{
		wxMenu* project = new wxMenu;
		project->Append(10001, wxT("New level\tCtrl+N"));
		project->Append(10002, wxT("Open level\tCtrl+O"));
		project->Append(10003, wxT("Save level\tCtrl+S"));
		Append(project, "Level");

		Connect(10001, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MenuBar::NewLevel));
		Connect(10002, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MenuBar::LoadLevel));
		Connect(10003, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MenuBar::SaveLevel));
	}


	MenuBar::~MenuBar()
	{
	}


	void MenuBar::BindCallbacks(std::function<void()>&& levelChangedCallback)
	{
		this->levelChangedCallback = std::move(levelChangedCallback);
	}


	void MenuBar::NewLevel(wxCommandEvent& e)
	{
		int response = wxMessageBox("Do you want to save the current level?", "Save", wxICON_WARNING | wxYES | wxNO | wxCENTER);

		if (response == wxYES) {
			SaveLevel(e);
		}

		wxTextEntryDialog levelNameDialog(this, "Level name:", "New Level");
		levelNameDialog.SetTextValidator(wxFILTER_EMPTY);

		if (levelNameDialog.ShowModal() == wxID_CANCEL)
			return;
		
		EditorData::NewLevel(wxStr2StdStr(levelNameDialog.GetValue()));
		levelChangedCallback();
	}


	void MenuBar::SaveLevel(wxCommandEvent& WXUNUSED(e))
	{
		if (EditorData::GetLevel()->HasPath()) {
			EditorData::GetLevel()->Save();
		}
		else {
			wxFileDialog saveDialog(this, "Save Level", "../Levels", "",
				"Sharpheus level file(*.lvl.sharpheus) | *.lvl.sharpheus", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

			if (saveDialog.ShowModal() == wxID_CANCEL)
				return;

			bool success = EditorData::GetLevel()->Save(wxStr2StdStr(saveDialog.GetPath()));
			SPHE_ASSERT(success, "Cannot save level. Check the log files for more information");
		}
	}


	void MenuBar::LoadLevel(wxCommandEvent& e)
	{
		int response = wxMessageBox("Do you want to save the current level?", "Save", wxICON_WARNING | wxYES | wxNO | wxCENTER);

		if (response == wxYES) {
			SaveLevel(e);
		}

		wxFileDialog loadDialog(this, "Open Level", "../Levels", "",
			"Sharpheus level file(*.lvl.sharpheus) | *.lvl.sharpheus", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

		if (loadDialog.ShowModal() == wxID_CANCEL)
			return;

		bool success = EditorData::GetLevel()->Load(wxStr2StdStr(loadDialog.GetPath()));
		SPHE_ASSERT(success, "Cannot load level. Check the log files for more information");
		levelChangedCallback();
	}
}
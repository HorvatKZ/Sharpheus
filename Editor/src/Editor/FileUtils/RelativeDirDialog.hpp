#pragma once

#include "editor_pch.h"

namespace Sharpheus {

	class RelativeDirDialog
	{
	public:
		RelativeDirDialog(wxWindow* parent, const wxString& title, const wxString& dir);
		virtual ~RelativeDirDialog();

		bool Show();
		inline const wxString& GetPath() { return path; }

	private:
		wxWindow* parent;
		wxString title, dir;
		wxString path;
	};


	inline RelativeDirDialog::RelativeDirDialog(wxWindow* parent, const wxString& title, const wxString& dir)
		: parent(parent), title(title), dir(dir)
	{
	}


	inline RelativeDirDialog::~RelativeDirDialog()
	{
	}


	inline bool RelativeDirDialog::Show()
	{
		int result = wxID_OK;
		while (result == wxID_OK) {
			wxDirDialog dialog(parent, title, dir, wxDD_DIR_MUST_EXIST);
			result = dialog.ShowModal();

			if (result == wxID_OK) {
				if (dialog.GetPath().Left(dir.Length()) == dir) {
					path = dialog.GetPath().Mid(dir.Length() + 1);
					return true;
				} else {
					wxMessageBox("The choosen file must be under " + dir, "Error", wxICON_ERROR | wxOK | wxCENTRE);
				}
			}
		}
		return false;
	}

}
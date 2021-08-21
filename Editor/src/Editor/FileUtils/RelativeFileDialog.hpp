#pragma once

#include "editor_pch.h"

namespace Sharpheus {

	template <bool isSave>
	class RelativeFileDialog
	{
	public:
		RelativeFileDialog(wxWindow* parent, const wxString& title, const wxString& dir, const wxString& types, const wxString& defaultFile = "");
		virtual ~RelativeFileDialog();

		bool Show();
		inline const wxString& GetPath() { return path; }

	private:
		wxWindow* parent;
		wxString title, dir, types, defaultFile;
		wxString path;
	};


	template<bool isSave>
	inline RelativeFileDialog<isSave>::RelativeFileDialog(wxWindow* parent, const wxString& title, const wxString& dir, const wxString& types, const wxString& defaultFile)
		: parent(parent), title(title), dir(dir), types(types), defaultFile(defaultFile)
	{
	}


	template<bool isSave>
	inline RelativeFileDialog<isSave>::~RelativeFileDialog()
	{
	}


	template<bool isSave>
	inline bool RelativeFileDialog<isSave>::Show()
	{
		int result = wxID_OK;
		while (result == wxID_OK) {
			wxFileDialog dialog(parent, title, dir, defaultFile, types, isSave ? wxFD_SAVE | wxFD_OVERWRITE_PROMPT : wxFD_OPEN | wxFD_FILE_MUST_EXIST);
			result = dialog.ShowModal();

			if (result == wxID_OK) {
				if (dialog.GetPath().Left(dir.Length()) == dir) {
					path = dialog.GetPath().Mid(dir.Length());
					return true;
				} else {
					wxMessageBox("The choosen file must be under " + dir, "Error", wxICON_ERROR | wxOK | wxCENTRE);
				}
			}
		}
		return false;
	}


	typedef RelativeFileDialog<true>	RelativeSaveDialog;
	typedef RelativeFileDialog<false>	RelativeOpenDialog;

}
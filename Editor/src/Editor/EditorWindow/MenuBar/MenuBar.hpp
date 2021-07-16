#pragma once

#include "editor_pch.h"


namespace Sharpheus {

	class MenuBar : public wxMenuBar
	{
	public:
		MenuBar(wxWindow* parent);
		virtual ~MenuBar();

	private:
		wxWindow* parent;

		void NewLevel(wxCommandEvent& e);
		void LoadLevel(wxCommandEvent& e);
		void SaveLevel(wxCommandEvent& e);
		void SaveLevelAs(wxCommandEvent& e);
		void AttachScene(wxCommandEvent& e);
		void SaveAsScene(wxCommandEvent& e);

		void ProjectSettings(wxCommandEvent& e);
		void WindowSettings(wxCommandEvent& e);
		void GridSettings(wxCommandEvent& e);
		void AnimatorCreator(wxCommandEvent& e);
		void TileSetCreator(wxCommandEvent& e);
		void ExportWithLogs(wxCommandEvent& e);
		void ExportFinal(wxCommandEvent& e);
		void About(wxCommandEvent& e);
	};

}
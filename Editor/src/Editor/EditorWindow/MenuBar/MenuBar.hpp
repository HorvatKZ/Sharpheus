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

		void ProjectSettings(wxCommandEvent& e);
		void WindowSettings(wxCommandEvent& e);

		void NewLevel(wxCommandEvent& e);
		void LoadLevel(wxCommandEvent& e);
		void SaveLevel(wxCommandEvent& e);
		void SaveLevelAs(wxCommandEvent& e);
		void AttachScene(wxCommandEvent& e);
		void SaveAsScene(wxCommandEvent& e);
		void LayerEditor(wxCommandEvent& e);

		void GridSettings(wxCommandEvent& e);
		void AnimatorCreator(wxCommandEvent& e);
		void TileSetCreator(wxCommandEvent& e);
		void ReloadAssets(wxCommandEvent& e);
		void ReloadScripts(wxCommandEvent& e);

		void ExportWithLogs(wxCommandEvent& e);
		void ExportFinal(wxCommandEvent& e);

		void About(wxCommandEvent& e);
	};

}
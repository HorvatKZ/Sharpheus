#pragma once

#include "editor_pch.h"


namespace Sharpheus {

	class MenuBar : public wxMenuBar
	{
	public:
		MenuBar(wxWindow* parent);
		virtual ~MenuBar();

		void BindCallbacks(std::function<void()>&& levelChangedCallback);

		void NewLevel(wxCommandEvent& e);
		void LoadLevel(wxCommandEvent& e);
		void SaveLevel(wxCommandEvent& e);
		void SaveLevelAs(wxCommandEvent& e);

	private:
		wxWindow* parent;
		std::function<void()> levelChangedCallback;

		void ProjectSettings(wxCommandEvent& e);
		void WindowSettings(wxCommandEvent& e);
		void EditorSettings(wxCommandEvent& e);
		void GridSettings(wxCommandEvent& e);
		void AnimatorCreator(wxCommandEvent& e);
		void ExportGame(wxCommandEvent& e);
	};

}
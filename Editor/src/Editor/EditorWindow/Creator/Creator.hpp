#pragma once

#include "editor_pch.h"
#include "ClassListCtrl.hpp"


namespace Sharpheus {

	class Creator : public wxPanel
	{
	public:
		Creator(wxFrame* parent, const wxPoint& pos, const wxSize& size, class LevelHierarchy* levelHierarchy);
		virtual ~Creator();

	private:
		wxTextCtrl* newName;
		wxBitmapButton* addButton;
		wxComboCtrl* typeSelector;
		ClassListCtrl* popup;

		class LevelHierarchy* levelHierarchy;
		GameObject::Type type = GameObject::Type::None;
		wxBitmap addButtonBitmap;

		static uint32_t border;
		static uint32_t textCtrlWidth;

		void OnAdd(wxCommandEvent& e);

		void TypeSelectorChanged(const wxString& name, GameObject::Type type);
		void SelectorDoubleClicked();
		void InitBitmaps();
	};

}
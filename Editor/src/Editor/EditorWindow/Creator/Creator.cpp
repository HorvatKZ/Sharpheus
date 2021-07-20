#include "editor_pch.h"
#include "Creator.hpp"
#include "ClassListCtrl.hpp"
#include "Editor/EditorWindow/LevelHierarchy.hpp"
#include "Editor/ResourceManagement/ImageManager.hpp"


namespace Sharpheus {

	const uint32 Creator::border = 2;
	const uint32 Creator::textCtrlWidth = 172;


	Creator::Creator(wxFrame* parent, const wxPoint& pos, const wxSize& size, LevelHierarchy* levelHierarchy)
		: wxPanel(parent, wxID_ANY, pos, size), levelHierarchy(levelHierarchy)
	{
		InitBitmaps();

		newName = new wxTextCtrl(this, wxID_ANY, "Unnamed", wxPoint(border, border), wxSize(textCtrlWidth, UI::unitHeight), wxTE_PROCESS_ENTER);
		addButton = new wxBitmapButton(this, wxID_ANY, addButtonBitmap, wxPoint(2 * border + textCtrlWidth, border), wxSize(UI::unitHeight, UI::unitHeight));
		typeSelector = new wxComboCtrl(this, wxID_ANY, "", wxPoint(border, 2 * border + UI::unitHeight), wxSize(textCtrlWidth + border + UI::unitHeight, UI::unitHeight));
		typeSelector->SetEditable(false);
		popup = new ClassListCtrl();
		typeSelector->SetPopupControl(popup);
		popup->InitContent(SPH_BIND_THIS_2(Creator::TypeSelectorChanged), SPH_BIND_THIS_0(Creator::SelectorDoubleClicked));

		addButton->Bind(wxEVT_BUTTON, &Creator::OnAdd, this);
		newName->Bind(wxEVT_TEXT_ENTER, &Creator::OnAdd, this);
	}

	Creator::~Creator()
	{
	}

	void Creator::OnAdd(wxCommandEvent& e)
	{
		wxString name = newName->GetValue();
		if (name.empty()) {
			SPHE_WARN("The name of the GameObject cannot be empty!");
		} else if (type == GameObject::Type::None) {
			typeSelector->SetValue("");
			SPHE_WARN("Choose a valid type before adding a GameObject!");
		} else {
			levelHierarchy->CreateNewGameObject(wxStr2StdStr(name), type);
			typeSelector->Dismiss();
		}
	}

	void Creator::TypeSelectorChanged(const wxString& name, GameObject::Type type)
	{
		this->type = type;
		typeSelector->SetValue(name);
	}

	void Creator::SelectorDoubleClicked()
	{
		if (type != GameObject::Type::None) {
			typeSelector->Dismiss();
		}
	}

	void Creator::InitBitmaps()
	{
		addButtonBitmap = ImageManager::GetImage("add.png", ImageManager::PathType::TOOLBAR);
	}

}
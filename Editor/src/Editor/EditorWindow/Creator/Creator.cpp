#include "editor_pch.h"
#include "Creator.hpp"
#include "ClassListCtrl.hpp"
#include "Editor/EditorWindow/LevelHierarchy.hpp"
#include "Editor/ResourceManagement/ImageManager.hpp"


namespace Sharpheus {

	Creator::Creator(wxFrame* parent, const wxPoint& pos, const wxSize& size, LevelHierarchy* levelHierarchy)
		: wxPanel(parent, wxID_ANY, pos, size), levelHierarchy(levelHierarchy)
	{
		InitBitmaps();

		newName = new wxTextCtrl(this, wxID_ANY, "Unnamed", wxPoint(2, 2), wxSize(172, 22), wxTE_PROCESS_ENTER);
		addButton = new wxBitmapButton(this, wxID_ANY, addButtonBitmap, wxPoint(176, 2), wxSize(22, 22));
		typeSelector = new wxComboCtrl(this, wxID_ANY, "", wxPoint(2, 26), wxSize(196, 22));
		typeSelector->SetEditable(false);
		popup = new ClassListCtrl();
		typeSelector->SetPopupControl(popup);
		popup->InitContent(SPH_BIND_THIS_2(Creator::TypeSelectorChanged));

		addButton->Bind(wxEVT_BUTTON, &Creator::OnAdd, this);
		newName->Bind(wxEVT_TEXT_ENTER, &Creator::OnAdd, this);
	}

	Creator::~Creator()
	{
	}

	void Creator::OnAdd(wxCommandEvent& e)
	{
		if (type == GameObject::Type::None) {
			typeSelector->SetValue("");
			SPHE_WARN("Choose a valid type before adding a GameObject!");
		} else {
			levelHierarchy->CreateNewGameObject(wxStr2StdStr(newName->GetValue()), type);
			typeSelector->Dismiss();
		}
	}

	void Creator::TypeSelectorChanged(const wxString& name, GameObject::Type type)
	{
		this->type = type;
		typeSelector->SetValue(name);
	}

	void Creator::InitBitmaps()
	{
		addButtonBitmap = ImageManager::GetImage("add.png", ImageManager::PathType::TOOLBAR);
	}

}
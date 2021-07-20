#pragma once

#include "editor_pch.h"
#include <wx/treectrl.h>


namespace Sharpheus {

	class LevelHierarchy : public wxTreeCtrl
	{
	public:
		LevelHierarchy(wxFrame* parent, const wxPoint& pos, const wxSize& size);
		virtual ~LevelHierarchy();

		void BindCallbacks(std::function<void()>&& currChangedCallback);

		void CreateNewGameObject(const std::string& name, GameObject::Type type);
		void FillWith(class GameObject* root);
		void CurrentChanged(class GameObject* curr);
		void CurrentNameChanged(const std::string& oldName, const std::string& newName);

	private:
		void Clear();
		void AddChildren(wxTreeItemId parentID, class GameObject* parent);
		void DeleteAllChildrenName(class GameObject* parent);
		void DuplicateCurrent();
		void DeselectCurrent();
		void DeleteCurrent();
		void OnSelectionChange(wxTreeEvent& e);
		void OnKeyPress(wxKeyEvent& e);
		void OnDragBegin(wxTreeEvent& e);
		void OnDragEnd(wxTreeEvent& e);
		GameObject* dragTarget = nullptr;

		std::unordered_map<wxString, wxTreeItemId> nameToId;
		std::unordered_map<GameObject::Type, uint32> typeToIcon;
		std::function<void()> currChangedCallback = nullptr;
	};

}
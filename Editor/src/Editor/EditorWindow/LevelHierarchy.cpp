#include "editor_pch.h"
#include "LevelHierarchy.hpp"
#include "Editor/Registry/EditorData.hpp"
#include "Editor/Registry/ProjectData.hpp"
#include "Editor/Registry/ClassRegistry.hpp"
#include "Editor/ResourceManagement/ImageManager.hpp"
#include <wx/kbdstate.h>


namespace Sharpheus {

	LevelHierarchy::LevelHierarchy(wxFrame* parent, const wxPoint& pos, const wxSize& size)
		: wxTreeCtrl(parent, 10001, pos, size, wxTR_TWIST_BUTTONS | wxTR_NO_LINES | wxTR_HAS_BUTTONS | wxTR_LINES_AT_ROOT)
	{
		uint32_t i = 0;
		wxImageList* images = new wxImageList(16, 16);
		auto iconPaths = ClassRegistry::GetAllIconPaths();
		for (auto it = iconPaths.begin(); it != iconPaths.end(); ++it) {
			images->Add(ImageManager::GetImage((*it).second, ImageManager::PathType::GAMEOBJECTS));
			typeToIcon[(*it).first] = i++;
		}
		AssignImageList(images);

		Bind(wxEVT_TREE_SEL_CHANGED, &LevelHierarchy::OnSelectionChange, this);
		Bind(wxEVT_KEY_DOWN, &LevelHierarchy::OnKeyPress, this);
		Bind(wxEVT_TREE_BEGIN_DRAG, &LevelHierarchy::OnDragBegin, this);
		Bind(wxEVT_TREE_END_DRAG, &LevelHierarchy::OnDragEnd, this);
	}


	LevelHierarchy::~LevelHierarchy()
	{
	}


	void LevelHierarchy::BindCallbacks(std::function<void()>&& currChangedCallback)
	{
		this->currChangedCallback = std::move(currChangedCallback);
	}


	void LevelHierarchy::CreateNewGameObject(const std::string& name, GameObject::Type type)
	{
		wxTreeItemId selected = GetSelection();
		if (!selected.IsOk()) {
			selected = GetRootItem();
		}
		GameObject* parent = ProjectData::GetLevel()->GetGameObject(wxStr2StdStr(GetItemText(selected)));

		GameObject* newObj = ProjectData::GetLevel()->Create(type, parent, name);
		SPHE_ASSERT(newObj != nullptr, "Cannot create GameObject from unexpected type");

		if (newObj != nullptr) {
			wxTreeItemId childID = AppendItem(selected, newObj->GetName(), typeToIcon[newObj->GetType()]);
			nameToId[newObj->GetName()] = childID;
			SelectItem(childID);
		}
	}


	void LevelHierarchy::FillWith(GameObject* root)
	{
		Clear();

		wxTreeItemId rootID = AddRoot(root->GetName(), typeToIcon[root->GetType()]);
		nameToId[root->GetName()] = rootID;
		AddChildren(rootID, root);
	}


	void LevelHierarchy::Clear()
	{
		nameToId.clear();
		DeleteAllItems();
	}


	void LevelHierarchy::CurrentChanged(GameObject* curr)
	{
		if (curr != nullptr) {
			wxTreeItemId current = nameToId[curr->GetName()];
			SelectItem(current);
		}
	}


	void LevelHierarchy::CurrentNameChanged(const std::string& oldName, const std::string& newName)
	{
		wxTreeItemId selected = GetSelection();
		nameToId.erase(oldName);
		nameToId[newName] = selected;
		SetItemText(selected, newName);
	}


	void LevelHierarchy::AddChildren(wxTreeItemId parentID, GameObject* parent)
	{
		for (GameObject* child : parent->GetChildren()) {
			wxTreeItemId childID = AppendItem(parentID, child->GetName(), typeToIcon[child->GetType()]);
			nameToId[child->GetName()] = childID;
			AddChildren(childID, child);
		}
	}


	void LevelHierarchy::DeleteAllChildrenName(GameObject* parent)
	{
		for (GameObject* child : parent->GetChildren()) {
			nameToId.erase(child->GetName());
			DeleteAllChildrenName(child);
		}
	}


	void LevelHierarchy::DuplicateCurrent()
	{
		wxTreeItemId selected = GetSelection();
		if (!selected.IsOk()) {
			return;
		}

		GameObject* curr = EditorData::GetCurrent();
		if (curr->GetParent() == nullptr) {
			SPHE_ERROR("Cannot duplicate root GameObject");
			return;
		}

		GameObject* duplicate = ProjectData::GetLevel()->Create(curr, curr->GetParent(), curr->GetName());
		duplicate->CopyFrom(curr);

		wxTreeItemId parent = GetItemParent(selected);
		wxTreeItemId duplicateID = AppendItem(parent, duplicate->GetName(), typeToIcon[duplicate->GetType()]);
		nameToId[duplicate->GetName()] = duplicateID;
		SelectItem(duplicateID);
		AddChildren(duplicateID, duplicate);
		currChangedCallback();
	}


	void LevelHierarchy::DeselectCurrent()
	{
		Unselect();
		EditorData::SetCurrent(nullptr);
		currChangedCallback();
	}


	void LevelHierarchy::DeleteCurrent()
	{
		GameObject* curr = EditorData::GetCurrent();
		if (curr->GetParent() == nullptr) {
			SPHE_ERROR("Cannot delete root GameObject");
			return;
		}

		if (curr->IsParentOfCurrentCamera()) {
			SPHE_ERROR("Cannot delete the current Camera, nor parent of the current Camera");
			return;
		}

		wxTreeItemId currItem = GetSelection();
		nameToId.erase(curr->GetName());
		DeleteAllChildrenName(curr);
		SelectItem(nameToId[curr->GetParent()->GetName()]);
		Delete(currItem);

		EditorData::SetCurrent(curr->GetParent());
		ProjectData::GetLevel()->Delete(curr);
		currChangedCallback();
	}


	void LevelHierarchy::OnSelectionChange(wxTreeEvent& e)
	{
		wxTreeItemId selected = GetSelection();
		EditorData::SetCurrent(GetItemText(selected));
		currChangedCallback();
	}


	void LevelHierarchy::OnKeyPress(wxKeyEvent& e)
	{
		switch (e.GetKeyCode()) {
			case WXK_DELETE:
				DeleteCurrent();
				break;

			case WXK_ESCAPE:
				DeselectCurrent();
				break;

			case 'D': case 'd':
				if (e.ControlDown()) {
					DuplicateCurrent();
				}
				break;
		}
	}


	void LevelHierarchy::OnDragBegin(wxTreeEvent& e)
	{
		dragTarget = ProjectData::GetLevel()->GetGameObject(wxStr2StdStr(GetItemText(e.GetItem())));
		if (dragTarget->GetParent() == nullptr) {
			dragTarget = nullptr;
		} else {
			e.Allow();
		}
	}


	void LevelHierarchy::OnDragEnd(wxTreeEvent& e)
	{
		if (dragTarget != nullptr) {
			wxTreeItemId newParentID = e.GetItem();
			GameObject* newParent = ProjectData::GetLevel()->GetGameObject(wxStr2StdStr(GetItemText(newParentID)));
			if (newParent == nullptr || newParent == dragTarget) {
				return;
			}

			ProjectData::GetLevel()->Move(dragTarget, newParent);

			Delete(nameToId[dragTarget->GetName()]);
			
			wxTreeItemId childID = AppendItem(newParentID, dragTarget->GetName(), typeToIcon[dragTarget->GetType()]);
			nameToId[dragTarget->GetName()] = childID;
			AddChildren(childID, dragTarget);
			SelectItem(childID);

			dragTarget = nullptr;
		}
	}

}
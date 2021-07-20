#include "pch.h"
#include "GameObject.hpp"
#include "Engine/Level.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"


namespace Sharpheus {

	const float GameObject::selectCircleRadius = 20.f;
	const Color GameObject::selectColor(255, 255, 0, 128);
	ID GameObject::nextFreeSafeObjectID = 1;


	GameObject::GameObject(GameObject* parent, const std::string& name) : parent(parent), name(name)
	{
		if (parent != nullptr) {
			parent->AddChild(this);
		} else {
			worldTrafo = trafo;
		}
	}


	GameObject::~GameObject()
	{
		while (!children.empty()) {
			delete children[0];
		}

		GameObjectDestroyedEvent e(this);
		for (auto it = safeObjects.begin(); it != safeObjects.end(); ++it) {
			(*it).second(e);
		}
		for (auto it = onDestroySubscribers.begin(); it != onDestroySubscribers.end(); ++it) {
			(*it).second(e);
		}

		if (parent != nullptr) {
			parent->RemoveChild(this);
		}

		if (level != nullptr) {
			level->Deregister(this);
		}
	}


	GameObject::GameObject(GameObject&& other)
		: parent(other.parent), name(other.name), trafo(other.trafo), worldTrafo(other.worldTrafo), level(other.level)
	{
		for (GameObject* child : other.children) {
			AddChild(child);
		}
		
		if (parent != nullptr) {
			parent->SetChildByName(this);
		}
		
		level->SetRegistry(this);

		other.children.clear();
		other.parent = nullptr;
		other.level = nullptr;
	}


	void GameObject::CopyFrom(GameObject* other)
	{
		SetTrafo(other->trafo);
		SetWorldTrafo(other->worldTrafo);

		for (GameObject* child : other->children) {
			GameObject* newChild = level->Create(child, this, child->name);
			newChild->CopyFrom(child);
		}
	}


	void GameObject::TickAll(float deltaTime)
	{
		Tick(deltaTime);

		for (uint32 i = 0; i < children.size(); ++i) {
			children[i]->TickAll(deltaTime);
		}
	}


	void GameObject::RenderAll()
	{
		if (isVisible) {
			Render();

			for (GameObject* child : children) {
				child->RenderAll();
			}
		}
	}


	void GameObject::RenderAsSelected()
	{
		Render();
		RenderSelection();
	}


	void GameObject::AddChild(GameObject* child)
	{
		children.push_back(child);
		child->parent = this;
		child->UpdateWorldTrafo(worldTrafo);
	}


	void GameObject::RemoveChild(GameObject* child)
	{
		auto placeInVector = std::find(children.begin(), children.end(), child);
		
		if (placeInVector != children.end()) {
			children.erase(placeInVector);
		} else {
			SPH_WARN("Attempt to remove non-existing child \"{0}\" from \"{1}\"", child->name, name);
		}
	}


	bool GameObject::IsSelected(const Point& pos)
	{
		if (!isVisible) {
			return false;
		}

		return (pos - worldTrafo.pos).Length() <= selectCircleRadius;
	}


	GameObject* GameObject::GetRoot()
	{
		GameObject* rootsParent = parent;
		GameObject* root = this;
		while (rootsParent != nullptr) {
			root = rootsParent;
			rootsParent = rootsParent->parent;
		}
		return root;
	}


	bool GameObject::IsDescendantOf(GameObject* other)
	{
		if (other == this) {
			return true;
		}

		if (parent == nullptr) {
			return false;
		}

		return parent->IsDescendantOf(other);
	}


	bool GameObject::Move(GameObject* newParent)
	{
		if (parent == nullptr) {
			SPH_ERROR("Cannot move root GameObject");
			return false;
		}

		if (newParent == nullptr) {
			SPH_ERROR("Cannot move GameObject to null parent");
			return false;
		}

		if (newParent->IsDescendantOf(this)) {
			SPH_ERROR("Cannot move GameObject under its descandant");
			return false;
		}

		parent->RemoveChild(this);
		newParent->AddChild(this);
		return true;
	}


	void GameObject::SetTrafo(const Transform& trafo)
	{
		this->trafo = trafo;
		UpdateWorldTrafo(parent != nullptr ? parent->worldTrafo : Transform());
	}


	void GameObject::SetWorldTrafo(const Transform& worldTrafo)
	{
		Transform parentWorldTrafo = parent != nullptr ? parent->worldTrafo : Transform();
		this->trafo = worldTrafo.SubstractFirst(parentWorldTrafo);
		UpdateWorldTrafo(parentWorldTrafo);
	}


	GameObject* GameObject::GetChild(const std::string& name)
	{
		uint32 i = 0;
		while (i < children.size() && name != children[i]->name) {
			++i;
		}

		if (i < children.size()) {
			return children[i];
		}

		return nullptr;
	}


	GameObject* GameObject::GetChild(uint32 ind)
	{
		if (ind < children.size()) {
			return children[ind];
		}

		return nullptr;
	}


	GameObject* GameObject::GetLastChild()
	{
		if (children.empty()) {
			return nullptr;
		}

		return children[children.size() - 1];
	}


	GameObject* GameObject::GetFirstChildOfType(Type type)
	{
		uint32 i = 0;
		while (i < children.size() && !children[i]->Is(type)) {
			++i;
		}

		if (i < children.size()) {
			return children[i];
		}

		return nullptr;
	}


	GameObject* GameObject::GetLastChildOfType(Type type)
	{
		if (children.empty()) {
			return nullptr;
		}

		uint32 i = children.size() - 1;
		while (i > 0 && !children[i]->Is(type)) {
			--i;
		}

		if (children[i]->Is(type)) {
			return children[i];
		}

		return nullptr;
	}


	GameObject* GameObject::GetFirstChildOfMask(TypeMasks mask)
	{
		uint32 i = 0;
		while (i < children.size() && !children[i]->Is(mask)) {
			++i;
		}

		if (i < children.size()) {
			return children[i];
		}

		return nullptr;
	}


	GameObject* GameObject::GetLastChildOfMask(TypeMasks mask)
	{
		if (children.empty()) {
			return nullptr;
		}

		uint32 i = children.size() - 1;
		while (i > 0 && !children[i]->Is(mask)) {
			--i;
		}

		if (children[i]->Is(mask)) {
			return children[i];
		}

		return nullptr;
	}


	void GameObject::SetName(const std::string& name)
	{
		this->name = level->RenameGameObject(this, name);
	}


	void GameObject::SetUpName(const std::string& name)
	{
		this->name = level->RenameGameObject(this, name, false);
	}


	void GameObject::SetChildByName(GameObject* newChild)
	{
		uint32 i = 0;
		while (i < children.size() && children[i]->name != newChild->name) {
			++i;
		}

		if (i < children.size()) {
			children[i] = newChild;
		}

		SPH_ASSERT(i != children.size(), "SetChildByName called to non children object. \"{0}\" has no child \"{1}\"", name, newChild->name);
	}


	bool GameObject::IsParentOfCurrentCamera()
	{
		if (IsCurrentCamera()) {
			return true;
		}

		uint32 i = 0;
		while (i < children.size() && !children[i]->IsParentOfCurrentCamera()) {
			++i;
		}

		return i < children.size();
	}

	GameObject* GameObject::GetUpperMostSelected(const Point& pos)
	{
		if (!isVisible) {
			return nullptr;
		}

		if (IsSelected(pos)) {
			return this;
		}

		if (!children.empty()) {
			uint32 i = children.size() - 1;
			while (i > 0) {
				GameObject* childSelected = children[i]->GetUpperMostSelected(pos);
				if (childSelected != nullptr) {
					return childSelected;
				}
				--i;
			}
			return children[0]->GetUpperMostSelected(pos);
		}

		return nullptr;
	}


	void GameObject::UpdateLastPosAll()
	{
		lastPos = worldTrafo.pos;

		for (GameObject* child : children) {
			child->UpdateLastPosAll();
		}
	}


	bool GameObject::SaveAll(FileSaver& fs)
	{
		bool success = true;
		success &= Save(fs);
		success &= fs.WriteEnd();

		uint32 i = 0;
		while (i < children.size() && success) {
			success &= children[i]->SaveAll(fs);
			++i;
		}

		SPH_ASSERT(success, "Error during saving GameObject \"{0}\"", name);
		return success;
	}


	bool GameObject::Load(FileLoader& fl)
	{
		Transform newTrafo;
		fl.Read(newTrafo);
		SetTrafo(newTrafo);
		fl.Read(isVisible);
		return fl.GetStatus();
	}


	bool GameObject::Save(FileSaver& fs)
	{
		fs.Write((byte)GetType());
		fs.Write(name);
		fs.Write((uint32)children.size());
		fs.Write(trafo);
		fs.Write(isVisible);
		return fs.GetStatus();
	}


	void GameObject::Tick(float deltaTime)
	{
	}


	void GameObject::Render()
	{
	}


	void GameObject::RenderSelection()
	{
		Point coords[] = {
			worldTrafo.pos + Point(-selectCircleRadius, -selectCircleRadius),
			worldTrafo.pos + Point(selectCircleRadius, -selectCircleRadius),
			worldTrafo.pos + Point(selectCircleRadius, selectCircleRadius),
			worldTrafo.pos + Point(-selectCircleRadius, selectCircleRadius)
		};
		ResourceManager::GetCircle()->Render(coords, selectColor);
	}


	void GameObject::UpdateWorldTrafo(const Transform& parentWorldTrafo)
	{
		worldTrafo = parentWorldTrafo + trafo;
		for (GameObject* child : children) {
			child->UpdateWorldTrafo(worldTrafo);
		}
	}

}
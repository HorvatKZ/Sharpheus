#include "pch.h"
#include "GameObject.hpp"


namespace Sharpheus {

	GameObject::GameObject(GameObject* parent, const std::string& name, const Transform& trafo) : parent(parent), name(name), trafo(trafo)
	{
		if (parent != nullptr) {
			parent->AddChild(this);
		} else {
			worldTrafo = trafo;
		}
	}


	GameObject::~GameObject()
	{
		for (GameObject* child : children) {
			delete child;
		}
	}


	void GameObject::Tick(float deltaTime)
	{
		TickThis(deltaTime);

		for (GameObject* child : children) {
			child->Tick(deltaTime);
		}
	}


	void GameObject::Render()
	{
		RenderThis();
		
		for (GameObject* child : children) {
			child->Render();
		}
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
		uint32_t i = 0;
		while (i < children.size() && name != children[i]->name) {
			++i;
		}

		if (i < children.size()) {
			return children[i];
		}

		return nullptr;
	}


	void GameObject::UpdateWorldTrafo(const Transform& parentWorldTrafo)
	{
		worldTrafo = parentWorldTrafo + trafo;
		for (GameObject* child : children) {
			child->UpdateWorldTrafo(worldTrafo);
		}
	}

}
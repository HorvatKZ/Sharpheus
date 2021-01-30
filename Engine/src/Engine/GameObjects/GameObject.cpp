#include "pch.h"
#include "GameObject.hpp"


namespace Sharpheus {

	GameObject::GameObject(const std::string& name, const Transform& trafo) : name(name), trafo(trafo)
	{
		worldTrafo = (parent != nullptr ? parent->worldTrafo : Transform()) + trafo;
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


	void GameObject::UpdateWorldTrafo(const Transform& parentWorldTrafo)
	{
		worldTrafo = parentWorldTrafo + trafo;
		for (GameObject* child : children) {
			child->UpdateWorldTrafo(worldTrafo);
		}
	}

}
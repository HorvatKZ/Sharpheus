#pragma once

#include "pch.h"
#include "Engine/Events/EventListener.hpp"


namespace Sharpheus {

	class SPH_EXPORT GameObject : public EventListener
	{
	public:
		GameObject(const std::string& name, const Transform& trafo);
		virtual ~GameObject();

		void Tick(float deltaTime);
		void Render();

		inline GameObject*		GetParent() { return parent; }
		void					AddChild(GameObject* child);
		GameObject*				GetRoot();

		inline Transform& GetTrafo() { return trafo; }
		inline Transform& GetWorldTrafo() { return worldTrafo; }
		virtual void SetTrafo(const Transform& trafo);

	protected:
		std::string name;
		Transform trafo;
		Transform worldTrafo;
		
		GameObject* parent = nullptr;
		std::vector<GameObject*> children;

		virtual void TickThis(float deltaTime) = 0;
		virtual void RenderThis() = 0;

		virtual void UpdateWorldTrafo(const Transform& parentWorldTrafo);
	};

}
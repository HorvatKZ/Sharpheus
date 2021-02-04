#pragma once

#include "Engine/Events/EventListener.hpp"

#define SPH_DECL_GAMEOBJECT(type) \
	virtual inline Type GetType() override { return Type::type; }


namespace Sharpheus {

	class SPH_EXPORT GameObject : public EventListener
	{
	public:
		enum class Type {
			None,
			Collection,
			Camera,
			Sprite,
			PhysicsObject, BoxCollider,
			Behavior, LocalListenerBehavior
		};

		GameObject(GameObject* parent, const std::string& name, const Transform& trafo);
		virtual ~GameObject();

		void Tick(float deltaTime);
		void Render();

		inline GameObject*		GetParent() { return parent; }
		void					AddChild(GameObject* child);
		void					RemoveChild(GameObject* child);
		GameObject*				GetRoot();

		inline Transform& GetTrafo() { return trafo; }
		inline Transform& GetWorldTrafo() { return worldTrafo; }
		virtual void SetTrafo(const Transform& trafo);
		virtual void SetWorldTrafo(const Transform& trafo);
		
		GameObject* GetChild(const std::string& name);
		inline const std::vector<GameObject*> GetChildren() { return children; }
		inline std::string GetName() { return name; }
		inline void SetName(const std::string& name) { this->name = name; }

		virtual inline Type GetType() = 0;

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
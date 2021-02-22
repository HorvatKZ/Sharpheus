#pragma once

#include "pch.h"
#include "GameObjects/GameObjects.h"


namespace Sharpheus {

	class SPH_EXPORT Level
	{
	public:
		Level(const std::string& name);
		virtual ~Level();

		void Tick(float deltaTime);
		void Render();

		// Contols
		template <class Class>
		Class* Create(GameObject* parent, const std::string& newName)
		{
			if (parent == nullptr) {
				SPH_ERROR("Cannot create GameObject without parent");
				return nullptr;
			}

			std::string correctNewName = GenerateUniqueName(newName);
			Class* newObj = new Class(parent, correctNewName);
			gameObjects[correctNewName] = newObj;
			newObj->SetLevel(this);
			return newObj;
		}

		void Delete(GameObject* obj);
		void Move(GameObject* obj, GameObject* newParent);
		std::string RenameGameObject(GameObject* obj, const std::string& newName);
		void Deregister(class GameObject* obj);

		inline class GameObject* GetRoot() { return root; }

		inline class GameObject* GetGameObject(const std::string& name) {
			auto it = gameObjects.find(name);
			if (it == gameObjects.end()) {
				return nullptr;
			}

			return gameObjects[name];
		}

	private:
		std::string name;
		class GameObject* root;
		std::unordered_map<std::string, class GameObject*> gameObjects;

		void RegisterWithUniqueName(class GameObject* newObject);

		std::string GenerateUniqueName(const std::string& originalName);
		bool		IsNameFree(const std::string& name);
	};

}
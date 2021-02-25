#pragma once

#include "pch.h"
#include "GameObjects/GameObjects.h"
#include "CollisionSystem/CollisionSystem.hpp"


namespace Sharpheus {

	class SPH_EXPORT Level
	{
	public:
		Level();
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

		inline CollisionSystem& GetCollSys() { return collSys; }
		inline class GameObject* GetRoot() { return root; }

		inline class GameObject* GetGameObject(const std::string& name) {
			auto it = gameObjects.find(name);
			if (it == gameObjects.end()) {
				return nullptr;
			}

			return gameObjects[name];
		}

		inline bool HasPath() { return !path.empty(); }
		inline const std::string& GetPath() { return path; }
		bool Save();
		bool Save(const std::string& filepath);
		bool Load(const std::string& filepath);


	private:
		std::string name;
		std::string path;
		class GameObject* root;
		CollisionSystem collSys;
		std::unordered_map<std::string, class GameObject*> gameObjects;

		bool SaveLevelData(FileSaver& file);
		bool LoadLevelData(FileLoader& file);
		bool LoadRoot(FileLoader& file);
		bool LoadGameObject(FileLoader& file, GameObject* parent);

		void RegisterWithUniqueName(class GameObject* newObject);

		std::string GenerateUniqueName(const std::string& originalName);
		bool		IsNameFree(const std::string& name);
	};

}
#pragma once

#include "pch.h"


namespace Sharpheus {

	class SPH_EXPORT Level
	{
	public:
		Level(const std::string& name);
		virtual ~Level();

		void Tick(float deltaTime);
		void Render();

		void Attach(class GameObject* newObject);
		void Detach(class GameObject* parent);

		inline class GameObject* GetGameObject(const std::string& name) {
			return gameObjects[name];
		}

	private:
		std::string name;
		class GameObject* root;
		std::unordered_map<std::string, class GameObject*> gameObjects;

		void RegisterWithUniqueName(class GameObject* newObject);
		void Deregister(class GameObject* obj);

		std::string GenerateUniqueName(const std::string& originalName);
		bool		IsNameFree(const std::string& name);
	};

}
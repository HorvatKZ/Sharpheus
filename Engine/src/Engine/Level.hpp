#pragma once

#include "pch.h"
#include "GameObjects/GameObjects.h"
#include "CollisionSystem/CollisionSystem.hpp"
#include "BehaviorCreatorBase.hpp"


namespace Sharpheus {

	typedef std::function<void()> RenderFunc;

	class SPH_EXPORT Level
	{
	public:
		Level();
		Level(const std::string& name);
		virtual ~Level();

		void Tick(float deltaTime);
		void Render();

		// Contols
		GameObject* Create(GameObject::Type type, GameObject* parent, const std::string& newName);
		GameObject* Create(GameObject* other, GameObject* parent, const std::string& newName);

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

		void Attach(GameObject* obj);

		void Delete(GameObject* obj, bool forceNow = false);
		bool Move(GameObject* obj, GameObject* newParent);
		std::string RenameGameObject(GameObject* obj, const std::string& newName, bool deregisterOld = true);
		void Deregister(GameObject* obj);
		void SetRegistry(GameObject* obj);

		inline void RequestInit(class Behavior* behavior) { behaviorsToInit.push(behavior); }

		inline CollisionSystem& GetCollSys() { return collSys; }
		inline GameObject* GetRoot() { return root; }

		inline GameObject* GetGameObject(const std::string& name) {
			auto it = gameObjects.find(name);
			if (it == gameObjects.end()) {
				return nullptr;
			}

			return gameObjects[name];
		}

		inline const std::string& GetName() { return name; }
		inline bool HasPath() { return !path.empty(); }
		inline const std::string& GetPath() { return path; }
		inline const std::string& GetFullPath() { return fullPath; }
		inline const std::string& GetProjectFileName() { return projectName; }
		inline void SetProjectFileName(const std::string& projectName) { this->projectName = projectName; }

		bool Save();
		bool Save(const std::string& base, const std::string& path);
		bool Load(const std::string& base, const std::string& path);
		bool LoadLevelData(const std::string& fullpath);

		bool CreateLayer(const std::string& name);
		bool CreateLayer(const std::string& name, uint32 ind);
		bool RenameLayer(uint32 ind, const std::string& newName);
		bool RenameLayer(const std::string& oldName, const std::string& newName);
		bool RemoveLayer(uint32 ind, uint32 moveToLayer);
		bool RemoveLayer(const std::string& name, const std::string& moveToLayer);
		bool SwapLayers(uint32 firstLayer, uint32 secondLayer);
		bool SwapLayers(const std::string& firstLayer, const std::string& secondLayer);
		bool AddToLayer(RenderableGameObject* obj, const std::string& layer);
		bool AddToLayer(ID id, RenderFunc&& func, const std::string& layer);
		bool RemoveFromLayers(RenderableGameObject* obj);
		bool RemoveFromLayers(ID id);
		bool IsLayerVisible(uint32 ind);
		void SetLayerVisible(uint32 ind, bool visiblity);
		inline bool IsLayerVisible(const std::string& name) { return IsLayerVisible(GetLayerInd(name)); }
		inline void SetLayerVisible(const std::string& name, bool visiblity) { SetLayerVisible(GetLayerInd(name), visiblity); }
		inline const std::vector<std::string>* GetLayerNames() { return &layerNames; }
		inline uint32 GetLayerNum() { return layers.size(); }
		static inline bool IsDeletableLayer(const std::string& layer) { return layer != "Default" && layer != "HUD"; }

		bool SaveAsScene(GameObject* obj, const std::string& path);
		bool AttachSceneTo(GameObject* obj, const std::string& path);

		static inline void SetBehaviorCreator(BehaviorCreatorBase* _bc) { bc = _bc; }

	private:
		struct Layer {
			std::string name;
			bool visible;
			std::unordered_set<RenderableGameObject*> objects;
			std::unordered_map<ID, RenderFunc> funcs;

			Layer(const std::string& name) : name(name), visible(true) {}
		};

		std::string name;
		std::string path, projectName, fullPath, base;
		GameObject* root;
		CollisionSystem collSys;
		std::unordered_map<std::string, GameObject*> gameObjects;
		std::vector<Layer> layers;
		std::vector<std::string> layerNames;
		std::queue<GameObject*> objsToDelete;
		std::queue<class Behavior*> behaviorsToInit;

		static BehaviorCreatorBase* bc;

		bool SaveLevelData(FileSaver& file);
		bool LoadLevelData(FileLoader& file);
		bool LoadRoot(FileLoader& file);
		bool LoadGameObject(FileLoader& file, GameObject* parent);

		void InitBehaviors();
		void DeleteObjects();

		void RegisterWithUniqueName(GameObject* newObject);

		std::string GenerateUniqueName(const std::string& originalName);
		bool		IsNameFree(const std::string& name);

		inline uint32 GetLayerInd(const std::string& name) {
			uint32 i = 0;
			while (i < layers.size() && layers[i].name != name) {
				++i;
			}

			return i;
		}
	};

}
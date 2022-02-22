#include "pch.h"
#include "Level.hpp"
#include "GameObjects/GameObjects.h"
#include "FileUtils/FileLoader.hpp"
#include "FileUtils/FileSaver.hpp"


namespace Sharpheus {

	BehaviorCreatorBase* Level::bc = nullptr;


	Level::Level()
	{
		root = nullptr;
	}


	Level::Level(const std::string& name) : name(name)
	{
		root = new Collection(nullptr, name);
		gameObjects[name] = root;
		root->SetLevel(this);
		collSys.SetRoot(root);

		Camera* camera = Create<Camera>(root, "Camera");
		camera->SetCurrent();

		CreateLayer("Default");
		CreateLayer("HUD");
	}


	Level::~Level()
	{
		if (Renderer::IsInited() && Renderer::GetCamera() != nullptr && Renderer::GetCamera()->GetRoot() == root) {
			Renderer::SetCamera(nullptr);
		}
		RadioButton::Clear();
		delete root;
	}


	void Level::Tick(float deltaTime)
	{
		InitBehaviors();

		root->TickAll(deltaTime);
		collSys.Tick();
		root->UpdateLastPosAll();

		DeleteObjects();
	}


	void Level::Render()
	{
		for (uint32 i = 0; i < layers.size(); ++i) {
			if (layers[i].visible) {
				for (auto it = layers[i].objects.begin(); it != layers[i].objects.end(); ++it) {
					(*it)->RenderIfVisible();
				}

				for (auto it = layers[i].funcs.begin(); it != layers[i].funcs.end(); ++it) {
					it->second();
				}
			}
		}

		collSys.Render();
	}


	GameObject* Level::Create(GameObject::Type type, GameObject* parent, const std::string& newName)
	{
		switch (type) {
			case GameObject::Type::Collection:
				return Create<Collection>(parent, newName);
			case GameObject::Type::Camera:
				return Create<Camera>(parent, newName);
			case GameObject::Type::Sprite:
				return Create<Sprite>(parent, newName);
			case GameObject::Type::Quad:
				return Create<Quad>(parent, newName);
			case GameObject::Type::Text:
				return Create<Text>(parent, newName);
			case GameObject::Type::AnimatedSprite:
				return Create<AnimatedSprite>(parent, newName);
			case GameObject::Type::AnimationPlayer:
				return Create<AnimationPlayer>(parent, newName);
			case GameObject::Type::TileMap:
				return Create<TileMap>(parent, newName);
			case GameObject::Type::ParticleEmitter:
				return Create<ParticleEmitter>(parent, newName);
			case GameObject::Type::PhysicsObject:
				return Create<PhysicsObject>(parent, newName);
			case GameObject::Type::BoxCollider:
				return Create<BoxCollider>(parent, newName);
			case GameObject::Type::CircleCollider:
				return Create<CircleCollider>(parent, newName);
			case GameObject::Type::CapsuleCollider:
				return Create<CapsuleCollider>(parent, newName);
			case GameObject::Type::MusicPlayer:
				return Create<MusicPlayer>(parent, newName);
			case GameObject::Type::SoundEffects:
				return Create<SoundEffects>(parent, newName);
			case GameObject::Type::Button:
				return Create<Button>(parent, newName);
			case GameObject::Type::ImageButton:
				return Create<ImageButton>(parent, newName);
			case GameObject::Type::CheckBox:
				return Create<CheckBox>(parent, newName);
			case GameObject::Type::RadioButton:
				return Create<RadioButton>(parent, newName);
			case GameObject::Type::Behavior:
				return Create<PlaceholderBehavior>(parent, newName);
		}

		return nullptr;
	}


	GameObject* Level::Create(GameObject* other, GameObject* parent, const std::string& newName)
	{
		GameObject::Type type = other->GetType();
		if (type == GameObject::Type::Behavior) {
			SPH_ASSERT(bc != nullptr, "BehaviorCreator is not set for Level");
			GameObject* behavior = bc->Create(((Behavior*)other)->GetSubType(), parent, newName);
			Attach(behavior);
			return behavior;
		}
		return Create(type, parent, newName);
	}


	void Level::Attach(GameObject* obj)
	{
		if (obj == nullptr) {
			SPH_ERROR("Cannot attach nullptr");
			return;
		}

		if (obj->GetParent() == nullptr) {
			SPH_ERROR("Cannot attach another root GameObject");
			return;
		}

		std::string correctNewName = GenerateUniqueName(obj->GetName());
		obj->SetLevel(this);
		obj->SetUpName(correctNewName);
		gameObjects[correctNewName] = obj;

		for (GameObject* child : obj->GetChildren()) {
			Attach(child);
		}
	}


	void Level::Delete(GameObject* obj, bool forceNow)
	{
		if (forceNow) {
			delete obj;
		} else {
			objsToDelete.push(obj);
		}
	}


	std::string Level::RenameGameObject(GameObject* obj, const std::string& newName, bool deregisterOld)
	{
		if (deregisterOld) {
			Deregister(obj);
		}
		std::string correctNewName = GenerateUniqueName(newName);
		if (obj == root) {
			name = correctNewName;
		}
		gameObjects[correctNewName] = obj;
		return correctNewName;
	}


	bool Level::Move(GameObject* obj, GameObject* newParent)
	{
		if (obj == nullptr) {
			SPH_ERROR("Tried to move null GameObject");
			return false;
		}

		return obj->Move(newParent);
	}


	bool Level::Save()
	{
		SPH_ASSERT(HasPath(), "File not specified for saving level \"{0}\"", name);
		return Save(base, path);
	}


	bool Level::Save(const std::string& base, const std::string& path)
	{
		FileSaver fs((base + path).c_str());

		bool success = true;
		success &= SaveLevelData(fs);
		success &= root->SaveAll(fs);

		if (!HasPath() && success) {
			this->path = path;
			this->base = base;
			fullPath = base + path;
		}

		SPH_ASSERT(success, "An error occured during saving level \"{0}\" to \"{1}{2}\"", name, base, path);
		return success;
	}


	bool Level::Load(const std::string& base, const std::string& path)
	{
		Renderer::SetCamera(nullptr);
		delete root;
		collSys.Clear();
		RadioButton::Clear();
		layers.clear();
		layerNames.clear();

		FileLoader fl((base + path).c_str());

		if (!fl.GetStatus()) {
			SPH_ERROR("Connot open level file \"{0}\"", path);
			return false;
		}

		bool success = true;
		success &= LoadLevelData(fl);
		success &= LoadRoot(fl);

		if (success) {
			this->path = path;
			this->base = base;
			fullPath = base + path;
		}

		SPH_ASSERT(success, "An error occured during loading level \"{0}\" from \"{1}{2}\"", name, base, path);
		return success;
	}


	bool Level::LoadLevelData(const std::string& fullpath)
	{
		FileLoader fl(fullpath.c_str());

		bool success = LoadLevelData(fl);

		SPH_ASSERT(success, "An error occured during loading level data from \"{1}\"", name, fullpath);
		return success;
	}


	bool Level::CreateLayer(const std::string& name)
	{
		if (name.empty()) {
			SPH_ERROR("CreateLayer: Layer's name cannot be empty");
			return false;
		}

		uint32 ind = GetLayerInd(name);
		if (ind != layers.size()) {
			SPH_ERROR("CreateLayer: Layer \"{0}\" already exists", name);
			return false;
		}

		layers.push_back(Layer(name));
		layerNames.push_back(name);
		return true;
	}


	bool Level::CreateLayer(const std::string& name, uint32 index)
	{
		if (name.empty()) {
			SPH_ERROR("CreateLayer: Layer's name cannot be empty");
			return false;
		}

		uint32 ind = GetLayerInd(name);
		if (ind != layers.size()) {
			SPH_ERROR("CreateLayer: Layer \"{0}\" already exists", name);
			return false;
		}

		layers.insert(layers.begin() + index, Layer(name));
		layerNames.insert(layerNames.begin() + index, name);
		return true;
	}


	bool Level::RenameLayer(uint32 index, const std::string& newName)
	{
		uint32 ind = GetLayerInd(newName);
		if (ind != layers.size()) {
			SPH_ERROR("RenameLayer: New layer \"{0}\" already exists", newName);
			return false;
		}

		if (index >= layers.size()) {
			SPH_ERROR("RenameLayer: No layer with index \"{0}\" exists", index);
			return false;
		}

		if (!IsDeletableLayer(layerNames[index])) {
			SPH_ERROR("RenameLayer: Cannot rename \"Default\" and \"HUD\" layers");
			return false;
		}

		layers[index].name = newName;
		layerNames[index] = newName;
		for (auto it = layers[index].objects.begin(); it != layers[index].objects.end(); ++it) {
			(*it)->SetLayerByLevel(newName);
		}
		return true;
	}


	bool Level::RenameLayer(const std::string& oldName, const std::string& newName)
	{
		if (oldName.empty() || newName.empty()) {
			SPH_ERROR("RenameLayer: Layer's name cannot be empty");
			return false;
		}

		if (oldName == newName) {
			return true;
		}

		return RenameLayer(GetLayerInd(oldName), newName);
	}


	bool Level::RemoveLayer(uint32 index, uint32 moveToLayer)
	{
		if (!IsDeletableLayer(layerNames[index])) {
			SPH_ERROR("RemoveLayer: Cannot remove \"Default\" and \"HUD\" layers");
			return false;
		}

		if (index == moveToLayer) {
			SPH_ERROR("RemoveLayer: Layer to delete and move to layer are the same");
			return false;
		}

		if (index >= layers.size()) {
			SPH_ERROR("RemoveLayer: No layer to delete with index \"{0}\" exists", index);
			return false;
		}

		if (moveToLayer >= layers.size()) {
			SPH_ERROR("RemoveLayer: No layer to move to with index \"{0}\" exists", moveToLayer);
			return false;
		}

		for (auto it = layers[index].objects.begin(); it != layers[index].objects.end(); ++it) {
			layers[moveToLayer].objects.insert(*it);
			(*it)->SetLayerByLevel(layerNames[moveToLayer]);
		}
		layers.erase(layers.begin() + index);
		layerNames.erase(layerNames.begin() + index);
		return true;
	}


	bool Level::RemoveLayer(const std::string& name, const std::string& moveToLayer)
	{
		if (name.empty() || moveToLayer.empty()) {
			SPH_ERROR("RemoveLayer: Layer's name cannot be empty");
			return false;
		}

		return RemoveLayer(GetLayerInd(name), GetLayerInd(moveToLayer));
	}


	bool Level::SwapLayers(uint32 firstLayer, uint32 secondLayer)
	{
		if (firstLayer == secondLayer) {
			return true;
		}

		if (firstLayer >= layers.size()) {
			SPH_ERROR("SwapLayers: First layer with index \"{0}\" does not exist", firstLayer);
			return false;
		}

		if (secondLayer >= layers.size()) {
			SPH_ERROR("SwapLayers: Second layer with index \"{0}\" does not exist", secondLayer);
			return false;
		}

		std::swap(layers[firstLayer], layers[secondLayer]);
		std::swap(layerNames[firstLayer], layerNames[secondLayer]);
		return true;
	}


	bool Level::SwapLayers(const std::string& firstLayer, const std::string& secondLayer)
	{
		if (firstLayer.empty() || secondLayer.empty()) {
			SPH_ERROR("SwapLayers: Layer's name cannot be empty");
			return false;
		}

		return SwapLayers(GetLayerInd(firstLayer), GetLayerInd(secondLayer));
	}


	bool Level::AddToLayer(RenderableGameObject* obj, const std::string& layer)
	{
		if (layer.empty()) {
			SPH_ERROR("AddToLayer: Layer's name cannot be empty");
			return false;
		}

		uint32 ind = GetLayerInd(layer);
		if (ind == layers.size()) {
			SPH_ERROR("AddToLayer: Layer \"{0}\" does not exist", layer);
			return false;
		}

		if (!obj->GetLayer().empty()) {
			bool result = RemoveFromLayers(obj);
			if (!result) {
				return false;
			}
		}

		layers[ind].objects.insert(obj);
		obj->SetLayerByLevel(layer);
		return true;
	}


	bool Level::AddToLayer(ID id, RenderFunc&& func, const std::string& layer)
	{
		if (layer.empty()) {
			SPH_ERROR("AddToLayer: Layer's name cannot be empty");
			return false;
		}

		uint32 ind = GetLayerInd(layer);
		if (ind == layers.size()) {
			SPH_ERROR("AddToLayer: Layer \"{0}\" does not exist", layer);
			return false;
		}

		layers[ind].funcs.insert(std::make_pair(id, std::move(func)));
		return true;
	}


	bool Level::RemoveFromLayers(RenderableGameObject* obj)
	{
		if (obj->GetLayer().empty()) {
			return true;
		}

		uint32 ind = GetLayerInd(obj->GetLayer());
		if (ind == layers.size()) {
			SPH_ERROR("RemoveFromLayers: Layer \"{0}\" does not exist", obj->GetLayer());
			return false;
		}

		layers[ind].objects.erase(obj);
		return true;
	}


	bool Level::RemoveFromLayers(ID id)
	{
		bool success = false;
		for (uint32 i = 0; i < layers.size(); ++i) {
			auto it = layers[i].funcs.find(id);
			if (it != layers[i].funcs.end()) {
				layers[i].funcs.erase(it);
				success = true;
			}
		}

		return success;
	}


	bool Level::IsLayerVisible(uint32 ind)
	{
		if (ind >= layers.size()) {
			SPH_ERROR("IsLayerVisible: No layer with index \"{0}\" exists", ind);
			return false;
		}
		return layers[ind].visible;
	}


	void Level::SetLayerVisible(uint32 ind, bool visiblity)
	{
		if (ind >= layers.size()) {
			SPH_ERROR("SetLayerVisible: No layer with index \"{0}\" exists", ind);
			return;
		}
		layers[ind].visible = visiblity;
	}


	bool Level::SaveAsScene(GameObject* obj, const std::string& path)
	{
		SPH_ASSERT(obj != nullptr, "Cannot save null GameObject as scene");

		std::string projPath = base.substr(0, base.length() - 7);
		FileSaver fs((projPath + "Scenes\\" + path).c_str());

		bool success = obj->SaveAll(fs);

		SPH_ASSERT(success, "An error occured during saving GameObject \"{0}\" as scene \"{1}\"", obj->GetName(), path);
		return success;
	}


	bool Level::AttachSceneTo(GameObject* obj, const std::string& path)
	{
		SPH_ASSERT(obj != nullptr, "Cannot load scene to  null GameObject");

		std::string projPath = base.substr(0, base.length() - 7);
		FileLoader fl((projPath + "Scenes\\" + path).c_str());

		bool success = LoadGameObject(fl, obj);

		SPH_ASSERT(success, "An error occured during loading scene \"{0}\" to GameObject \"{1}\"", path, obj->GetName());
		return success;
	}


	bool Level::SaveLevelData(FileSaver& fs)
	{
		fs.Write(name);
		fs.Write(projectName);
		fs.WriteEnd();
		fs.Write((uint32)layerNames.size());
		for (uint32 i = 0; i < layers.size(); ++i) {
			fs.Write(layers[i].name);
			fs.Write(layers[i].visible);
		}
		fs.WriteEnd();
		return fs.GetStatus();
	}


	bool Level::LoadLevelData(FileLoader& fl)
	{
		fl.Read(name);
		fl.Read(projectName);
		bool success = fl.TryReadingEnd();
		uint32 n;
		fl.Read(n);
		for (uint32 i = 0; i < n; ++i) {
			std::string layer;
			fl.Read(layer);
			CreateLayer(layer);
			bool isVisible;
			fl.Read(isVisible);
			SetLayerVisible(i, isVisible);
		}
		return success && fl.TryReadingEnd();
	}


	bool Level::LoadRoot(FileLoader& fl)
	{
		byte type;
		std::string rootName;
		uint32 childNum;

		bool success = true;
		success &= fl.Read(type);
		if (type != (byte)GameObject::Type::Collection) {
			SPH_ERROR("Wrong root object type");
			return false;
		}

		success &= fl.Read(rootName);
		root = new Collection(nullptr, rootName);
		gameObjects[rootName] = root;
		root->SetLevel(this);

		success &= fl.Read(childNum);
		success &= root->Load(fl);
		success &= fl.TryReadingEnd();

		uint32 i = 0;
		while (i < childNum && success) {
			success &= LoadGameObject(fl, root);
			++i;
		}

		collSys.SetRoot(root);

		SPH_ASSERT(success, "Error during loading root GameObject \"{0}\"", rootName);
		return success;
	}


	bool Level::LoadGameObject(FileLoader& fl, GameObject* parent)
	{
		byte type;
		GameObject::Type realType;
		std::string objName;
		uint32 childNum;

		bool success = true;
		success &= fl.Read(type);
		realType = (GameObject::Type)type;

		success &= fl.Read(objName);
		GameObject* obj;
		if (realType != GameObject::Type::Behavior) {
			obj = Create(realType, parent, objName);
		} else {
			uint32 subType;
			SPH_ASSERT(bc != nullptr, "BehaviorCreator is not set for Level");
			success &= fl.Read(subType);
			obj = bc->Create(subType, parent, objName);
			Attach(obj);
		}
		

		if (obj == nullptr) {
			SPH_ERROR("Could not create GameObject \"{0}\" with type {1}", objName, realType);
			return false;
		}

		success &= fl.Read(childNum);
		success &= obj->Load(fl);
		success &= fl.TryReadingEnd();

		uint32 i = 0;
		while (i < childNum && success) {
			success &= LoadGameObject(fl, obj);
			++i;
		}

		SPH_ASSERT(success, "Error during loading GameObject \"{0}\"", objName);
		return success;
	}


	void Level::InitBehaviors()
	{
		while (!behaviorsToInit.empty()) {
			Behavior* behavior = behaviorsToInit.front();
			behaviorsToInit.pop();
			behavior->Init();
		}
	}


	void Level::DeleteObjects()
	{
		while (!objsToDelete.empty()) {
			GameObject* obj = objsToDelete.front();
			objsToDelete.pop();
			if (!obj->IsParentOfCurrentCamera()) {
				delete obj;
			}
		}
	}


	void Level::RegisterWithUniqueName(GameObject* newObject)
	{
		std::string newName = GenerateUniqueName(newObject->GetName());
		newObject->SetName(newName);
		gameObjects[newName] = newObject;
	}


	void Level::Deregister(GameObject* obj)
	{
		auto it = gameObjects.find(obj->GetName());
		if (it != gameObjects.end()) gameObjects.erase(it);
	}


	void Level::SetRegistry(GameObject* obj)
	{
		gameObjects[obj->GetName()] = obj;
	}


	std::string Level::GenerateUniqueName(const std::string& originalName)
	{
		if (IsNameFree(originalName)) {
			return originalName;
		}


		int counter = 1;
		bool isFree = false;
		std::string result, basename = originalName;
		size_t position = originalName.find_last_of('_');
		if (position != std::string::npos) {
			try {
				counter = std::stoi(originalName.substr(position + 1));
				basename = originalName.substr(0, position);
			} catch (...) {}
		}
		while (!isFree) {
			result = basename + "_" + std::to_string(counter);
			isFree = IsNameFree(result);
			if (!isFree) {
				++counter;
			}
		}
		return result;
	}


	bool Level::IsNameFree(const std::string& name)
	{
		auto it = gameObjects.find(name);
		return it == gameObjects.end();
	}
}
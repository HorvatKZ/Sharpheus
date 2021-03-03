#include "pch.h"
#include "Level.hpp"
#include "GameObjects/GameObjects.h"
#include "FileUnits/FileLoader.hpp"
#include "FileUnits/FileSaver.hpp"
#include "BehaviorCreator.hpp"


namespace Sharpheus {

	Level::Level()
	{
		root = nullptr;
	}


	Level::Level(const std::string& name) : name(name)
	{
		root = new Collection(nullptr, name);
		gameObjects[name] = root;
		root->SetLevel(this);

		Camera* camera = Create<Camera>(root, "Camera");
		camera->SetCurrent();
	}


	Level::~Level()
	{
		if (Renderer::GetCamera()->GetRoot() == root) {
			Renderer::SetCamera(nullptr);
		}
		delete root;
	}


	void Level::Tick(float deltaTime)
	{
		collSys.Tick();
		root->TickAll(deltaTime);
	}


	void Level::Render()
	{
		root->RenderAll();
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
			case GameObject::Type::PhysicsObject:
				return Create<PhysicsObject>(parent, newName);
			case GameObject::Type::BoxCollider:
				return Create<BoxCollider>(parent, newName);
			case GameObject::Type::Behavior:
				return Create<PlaceholderBehavior>(parent, newName);
		}

		return nullptr;
	}


	GameObject* Level::Create(GameObject* other, GameObject* parent, const std::string& newName)
	{
		GameObject::Type type = other->GetType();
		if (type == GameObject::Type::Behavior) {
			GameObject* behavior = BehaviorCreator::Create(((Behavior*)other)->GetSubType(), parent, newName);
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


	void Level::Delete(GameObject* obj)
	{
		delete obj;
	}


	std::string Level::RenameGameObject(GameObject* obj, const std::string& newName, bool deregisterOld)
	{
		if (deregisterOld) {
			Deregister(obj);
		}
		std::string correctNewName = GenerateUniqueName(newName);
		gameObjects[correctNewName] = obj;
		return correctNewName;
	}


	void Level::Move(GameObject* obj, GameObject* newParent)
	{
		SPH_ASSERT(obj != nullptr, "Tried to move null GameObject");
		obj->Move(newParent);
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

		FileLoader fl((base + path).c_str());

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


	bool Level::SaveLevelData(FileSaver& fs)
	{
		fs.Write(name);
		fs.Write(projectPath);
		fs.WriteEnd();
		return fs.GetStatus();
	}


	bool Level::LoadLevelData(FileLoader& fl)
	{
		fl.Read(name);
		fl.Read(projectPath);
		return fl.TryReadingEnd();
	}


	bool Level::LoadRoot(FileLoader& fl)
	{
		uint8_t type;
		std::string rootName;
		uint32_t childNum;

		bool success = true;
		success &= fl.Read(type);
		if (type != (uint8_t)GameObject::Type::Collection) {
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

		uint32_t i = 0;
		while (i < childNum && success) {
			success &= LoadGameObject(fl, root);
			++i;
		}

		SPH_ASSERT(success, "Error during loading root GameObject \"{0}\"", rootName);
		return success;
	}


	bool Level::LoadGameObject(FileLoader& fl, GameObject* parent)
	{
		uint8_t type;
		GameObject::Type realType;
		std::string objName;
		uint32_t childNum;

		bool success = true;
		success &= fl.Read(type);
		realType = (GameObject::Type)type;

		success &= fl.Read(objName);
		GameObject* obj;
		if (realType != GameObject::Type::Behavior) {
			obj = Create(realType, parent, objName);
		} else {
			uint32_t subType;
			success &= fl.Read(subType);
			obj = BehaviorCreator::Create(subType, parent, objName);
			Attach(obj);
		}
		

		if (obj == nullptr) {
			SPH_ERROR("Could not create GameObject \"{0}\" with type {1}", objName, realType);
			return false;
		}

		success &= fl.Read(childNum);
		success &= obj->Load(fl);
		success &= fl.TryReadingEnd();

		uint32_t i = 0;
		while (i < childNum && success) {
			success &= LoadGameObject(fl, obj);
			++i;
		}

		SPH_ASSERT(success, "Error during loading GameObject \"{0}\"", objName);
		return success;
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
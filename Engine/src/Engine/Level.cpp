#include "pch.h"
#include "Level.hpp"
#include "GameObjects/GameObjects.h"
#include "GameObjects/Behaviors/DebugBehavior.hpp"
#include "GameObjects/Behaviors/PlayerController.hpp"
#include "FileUnits/FileLoader.hpp"
#include "FileUnits/FileSaver.hpp"


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
		//camera->SetUpRect(1280, 720);
		camera->SetCurrent();

		// For testing
		PhysicsObject* player = Create<PhysicsObject>(root, "Player");
		player->SetGravity(500);

		Sprite* playerSprite = Create<Sprite>(player, "Sprite");
		playerSprite->SetTrafo({ { 0, 0 }, {3, 3}, 0 });
		playerSprite->SetImageFromPath("../Assets/Testing/test.png", false);

		BoxCollider* playerCol = Create<BoxCollider>(player, "Box");
		playerCol->SetTrafo({ { 0, 0 }, {3, 3}, 0 });
		playerCol->SetRect(20, 32);

		//Create<PlayerController>(player, "Controller");

		Sprite* ground = Create<Sprite>(root, "Sprite2");
		ground->SetTrafo({ { 0, 300 }, {1, .1}, 0 });
		ground->SetImageFromPath("../Assets/Branding/sharpheus_promo.png", true);

		BoxCollider* box1 = Create<BoxCollider>(ground, "GroundBox1");
		box1->SetTrafo({ {0, 0}, {1, 1}, 0 });
		box1->SetRect(800, 600);
		BoxCollider* box2 = Create<BoxCollider>(ground, "GroundBox2");
		box2->SetTrafo({{-400, 0}, {1, 1}, 0 });
		box2->SetRect(60, 5000);
		BoxCollider* box3 = Create<BoxCollider>(ground, "GroundBox3");
		box3->SetTrafo({ {400, 0}, {1, 1}, 0 });
		box3->SetRect(60, 5000);

		//Create<DebugBehavior>(root, "Debug");
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


	void Level::Delete(GameObject* obj)
	{
		delete obj;
	}


	std::string Level::RenameGameObject(GameObject* obj, const std::string& newName)
	{
		Deregister(obj);
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
		return Save(path);
	}


	bool Level::Save(const std::string& filepath)
	{
		FileSaver fs(filepath.c_str());

		bool success = true;
		success &= SaveLevelData(fs);
		success &= root->SaveAll(fs);

		if (!HasPath() && success) {
			path = filepath;
		}

		SPH_ASSERT(success, "An error occured during saving level \"{0}\" to \"{1}\"", name, filepath);
		return success;
	}


	bool Level::Load(const std::string& filepath)
	{
		Renderer::SetCamera(nullptr);
		delete root;
		collSys.Clear();

		FileLoader fl(filepath.c_str());
		path = filepath;

		bool success = true;
		success &= LoadLevelData(fl);
		success &= LoadRoot(fl);

		SPH_ASSERT(success, "An error occured during loading level \"{0}\" from \"{1}\"", name, filepath);
		return success;
	}


	bool Level::SaveLevelData(FileSaver& fs)
	{
		fs.Write(name);
		fs.WriteEnd();
		return fs.GetStatus();
	}


	bool Level::LoadLevelData(FileLoader& fl)
	{
		fl.Read(name);
		return fl.TryReadingEnd();
	}


	bool Level::LoadRoot(FileLoader& fl)
	{
		uint8_t type;
		std::string rootName;
		uint32_t childNum;

		bool success = true;
		success &= fl.Read(type);
		SPH_ASSERT(type == (uint8_t)GameObject::Type::Collection, "Wrong root object type");

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
		GameObject* obj = nullptr;

		switch (realType) {
			case GameObject::Type::Collection:
				obj = Create<Collection>(parent, objName);
				break;
			case GameObject::Type::Camera:
				obj = Create<Camera>(parent, objName);
				break;
			case GameObject::Type::Sprite:
				obj = Create<Sprite>(parent, objName);
				break;
			case GameObject::Type::PhysicsObject:
				obj = Create<PhysicsObject>(parent, objName);
				break;
			case GameObject::Type::BoxCollider:
				obj = Create<BoxCollider>(parent, objName);
				break;
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


	std::string Level::GenerateUniqueName(const std::string& originalName)
	{
		if (IsNameFree(originalName)) {
			return originalName;
		}

		int counter = 1;
		bool isFree = false;
		std::string result;
		while (!isFree) {
			result = originalName + "_" + std::to_string(counter);
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
#include "pch.h"
#include "Level.hpp"
#include "GameObjects/GameObjects.h"
#include "GameObjects/Behaviors/DebugBehavior.hpp"
#include "GameObjects/Behaviors/PlayerController.hpp"
#include "CollisionSystem/CollisionSystem.hpp"


namespace Sharpheus {

	Level::Level(const std::string& name) : name(name)
	{
		CollisionSystem::Init();

		
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

		Create<PlayerController>(player, "Controller");

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

		Create<DebugBehavior>(root, "Debug");
	}


	Level::~Level()
	{
		delete root;
		CollisionSystem::Clear();
	}


	void Level::Tick(float deltaTime)
	{
		CollisionSystem::Tick();
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
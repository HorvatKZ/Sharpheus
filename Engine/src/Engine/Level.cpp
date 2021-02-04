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

		// For testing
		PhysicsObject* player = new PhysicsObject(root, "Player", { { 0, -200 }, {1, 1}, 0 });
		player->SetGravity(500);
		new Sprite(player, "Sprite", { { 0, 0 }, {3, 3}, 0 }, "../Assets/Testing/test.png");
		new BoxCollider(player, "Box", { { 0, 0 }, {3, 3}, 0 }, 20, 32);
		new PlayerController(player, "Controller", 200, 200);
		Attach(root, player);

		Sprite* ground = new Sprite(root, "Sprite2", { { 0, 300 }, {1, .1}, 0 }, "../Assets/Branding/sharpheus_promo.png");
		new BoxCollider(ground, "GroundBox1", { {0, 0}, {1, 1}, 0 }, 800, 600);
		new BoxCollider(ground, "GroundBox2", { {-400, 0}, {1, 1}, 0 }, 60, 5000);
		new BoxCollider(ground, "GroundBox3", { {400, 0}, {1, 1}, 0 }, 60, 5000);
		Attach(root, ground);

		Attach(root, new DebugBehavior(root, "Debug"));

		Camera* cam = new Camera(root, "Camera", { { 0, 0 }, {1, 1}, 0 }, 1280, 720);
		cam->SetCurrent();
		Attach(root, cam);
	}


	Level::~Level()
	{
		CollisionSystem::Clear();
		delete root;
	}


	void Level::Tick(float deltaTime)
	{
		CollisionSystem::Tick();
		root->Tick(deltaTime);
	}


	void Level::Render()
	{
		root->Render();
	}


	void Level::Attach(const std::string& parentName, GameObject* newObject)
	{
		Attach(GetGameObject(parentName), newObject);
	}


	void Level::Attach(GameObject* parent, GameObject* newObject)
	{
		parent->AddChild(newObject);
		RegisterWithUniqueName(newObject);
		for (GameObject* child : newObject->GetChildren()) {
			RegisterWithUniqueName(child);
		}
	}


	void Level::Detach(GameObject* newObject)
	{
		GameObject* parent = newObject->GetParent();
		SPH_ASSERT("Attempt to detach parentless child \"{0}\"", newObject->GetName());

		parent->RemoveChild(newObject);
		Deregister(newObject);
		for (GameObject* child : newObject->GetChildren()) {
			Deregister(child);
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
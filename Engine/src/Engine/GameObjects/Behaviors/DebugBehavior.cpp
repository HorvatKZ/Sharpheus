#include "pch.h"
#include "DebugBehavior.hpp"
#include "Engine/Events/KeyEvents.hpp"
#include "Engine/CollisionSystem/CollisionSystem.hpp"


using namespace Sharpheus;


DebugBehavior::DebugBehavior(GameObject* parent, const std::string& name) : Behavior(parent, name)
{
	/*Subscribe<KeyPressedEvent>([&](const KeyPressedEvent& e) {
		if (e.code == KeyCode::E) CollisionSystem::ShowColliders();
	});

	Subscribe<KeyReleasedEvent>([&](const KeyReleasedEvent& e) {
		if (e.code == KeyCode::E) CollisionSystem::HideColliders();
	});*/
}


DebugBehavior::~DebugBehavior()
{
}

void DebugBehavior::Tick(float deltaTime)
{
}

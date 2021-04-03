#include "pch.h"
#include "DebugBehavior.hpp"
#include "Engine/Events/KeyEvents.hpp"
#include "Engine/CollisionSystem/CollisionSystem.hpp"
#include "Engine/Level.hpp"


using namespace Sharpheus;


ClassInfo DebugBehavior::classInfo("DebugBehavior", "behavior.png", {});


DebugBehavior::DebugBehavior(Behavior* other) : Behavior(other)
{
	DoSubscriptions();
}


DebugBehavior::DebugBehavior(GameObject* parent, const std::string& name) : Behavior(parent, name)
{
	DoSubscriptions();
}


DebugBehavior::~DebugBehavior()
{
}


void DebugBehavior::Tick(float deltaTime)
{
}


void DebugBehavior::DoSubscriptions()
{
	Subscribe<KeyPressedEvent>([&](const KeyPressedEvent& e) {
		if (e.code == KeyCode::E) level->GetCollSys().ShowColliders();
		});

	Subscribe<KeyReleasedEvent>([&](const KeyReleasedEvent& e) {
		if (e.code == KeyCode::E) level->GetCollSys().HideColliders();
		});
}


bool DebugBehavior::IsCompatibleWithParent(GameObject* parent)
{
	return parent != nullptr;
}
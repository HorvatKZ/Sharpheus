#include "pch.h"
#include "TriggerTester.hpp"
#include "Engine/CollisionSystem/CollisionEvent.hpp"
#include "Engine/GameObjects/Renderables/Sprite.hpp"


using namespace Sharpheus;


ClassInfo TriggerTester::classInfo("TriggerTester", "behavior.png", {});


TriggerTester::TriggerTester(Behavior* other) : Behavior(other), ColliderListener(GetID())
{
	DoSubscriptions();
}


TriggerTester::TriggerTester(GameObject* parent, const std::string& name) : Behavior(parent, name), ColliderListener(GetID())
{
	DoSubscriptions();
}


TriggerTester::~TriggerTester()
{
}


void TriggerTester::Tick(float deltaTime)
{
}


void TriggerTester::DoSubscriptions()
{
	SubscribeOnTriggerEnter((Collider*)parent->GetFirstChildOfType(Type::CircleCollider), SPH_BIND(TriggerTester::OnTriggerEnter));
	SubscribeOnTriggerExit((Collider*)parent->GetFirstChildOfType(Type::CircleCollider), SPH_BIND(TriggerTester::OnTriggerExit));
}


void TriggerTester::OnTriggerEnter(const OnEnterEvent& e)
{
	if (e.obj->GetName() == "Target") {
		((Sprite*)parent)->SetTint(Color::Red);
	}
}


void TriggerTester::OnTriggerExit(const OnExitEvent& e)
{
	if (e.obj->GetName() == "Target") {
		((Sprite*)parent)->SetTint(Color::White);
	}
}


bool TriggerTester::IsCompatibleWithParent(GameObject* parent)
{
	if (parent == nullptr) {
		return false;
	}

	if (!parent->Is(Type::Sprite)) {
		return false;
	}

	if (parent->GetFirstChildOfType(Type::CircleCollider) == nullptr) {
		return false;
	}

	return true;
}
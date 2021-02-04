#include "pch.h"
#include "Collider.hpp"
#include "Engine/CollisionSystem/CollisionSystem.hpp"


namespace Sharpheus {

	Color Collider::shapeColor = Color(0, 255, 0, 127);


	Collider::Collider(GameObject* parent, const std::string& name, const Transform& trafo) : GameObject(parent, name, trafo)
	{
		prevPos = worldTrafo.pos;
		CollisionSystem::AddCollider(this);
	}

	Collider::~Collider()
	{
		CollisionSystem::RemoveCollider(this);

		LocalSourceDestroyedEvent e(GetID());
		for (auto it = subscribers.begin(); it != subscribers.end(); ++it) {
			(*it).second.second(e);
		}
	}

	void Collider::UpdatePrevPos()
	{
		prevPos = worldTrafo.pos;
	}

	void Collider::TickThis(float deltaTime)
	{
	}

	void Collider::RenderThis()
	{
		if (visible) {
			RenderShape();
		}
	}
}
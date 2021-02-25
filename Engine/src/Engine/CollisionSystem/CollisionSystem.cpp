#include "pch.h"
#include "CollisionSystem.hpp"
#include "Engine/GameObjects/Colliders/Collider.hpp"


namespace Sharpheus {

	CollisionSystem::CollisionSystem()
	{
	}


	CollisionSystem::~CollisionSystem()
	{
	}


	void CollisionSystem::Clear()
	{
		colliders.clear();
	}


	void CollisionSystem::Tick()
	{
		for (uint32_t i = 0; i < colliders.size() - 1; ++i) {
			for (uint32_t j = i + 1; j < colliders.size(); ++j) {
				if (!colliders[i]->WasStill() || !colliders[j]->WasStill()) {
					auto collision = colliders[i]->CalcCollision(colliders[j]);
					if (collision.first != Point::Zero && collision.second != Point::Zero) {
						CollisionEvent e1(colliders[i], colliders[j], collision.second);
						colliders[i]->OnCollision(e1);
						CollisionEvent e2(colliders[j], colliders[i], collision.first);
						colliders[j]->OnCollision(e2);
					}
				}
			}
		}

		for (Collider* collider : colliders) {
			collider->UpdatePrevPos();
		}
	}


	void CollisionSystem::AddCollider(Collider* collider)
	{
		colliders.push_back(collider);
		collider->SetVisible(areCollidersVisible);
	}


	void CollisionSystem::RemoveCollider(Collider* collider)
	{
		uint32_t i = 0;
		while (i < colliders.size() && (*collider) != (*colliders[i])) {
			++i;
		}

		if (i < colliders.size()) {
			colliders.erase(colliders.begin() + i);
		} else {
			SPH_WARN("Attempt to remove unregistered collider \"{0}\"", collider->GetName());
		}
	}


	void CollisionSystem::ShowColliders()
	{
		areCollidersVisible = true;
		for (Collider* collider : colliders) {
			collider->Show();
		}
	}


	void CollisionSystem::HideColliders()
	{
		areCollidersVisible = false;
		for (Collider* collider : colliders) {
			collider->Hide();
		}
	}

}
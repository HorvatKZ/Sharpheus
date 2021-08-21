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
		if (colliders.empty()) {
			return;
		}

		for (uint32 i = 0; i < colliders.size() - 1; ++i) {
			if (!colliders[i]->IsTrigger()) {
				for (uint32 j = i + 1; j < colliders.size(); ++j) {
					if (!colliders[j]->IsTrigger() && (!colliders[i]->WasStill() || !colliders[j]->WasStill())) {
						CollDataPair cds = colliders[i]->CalcCollision(colliders[j]);
						if (cds.first.Is()) {
							colliders[i]->OnCollision(cds.first, colliders[j]);
						}
						if (cds.second.Is()) {
							colliders[j]->OnCollision(cds.second, colliders[i]);
						}
					}
				}
			}
		}

		for (Collider* collider : colliders) {
			if (collider->IsTrigger()) {
				collider->CheckTriggering(root);
			}
		}

		for (Collider* collider : colliders) {
			collider->SwapMaps();
		}
	}


	void CollisionSystem::AddCollider(Collider* collider)
	{
		colliders.push_back(collider);
		collider->SetColliderVisible(areCollidersVisible);
	}


	void CollisionSystem::RemoveCollider(Collider* collider)
	{
		uint32 i = 0;
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
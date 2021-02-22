#pragma once

#include "Behavior.hpp"
#include "../Colliders/Collider.hpp"
#include "Engine/CollisionSystem/CollisionEvent.hpp"


namespace Sharpheus {

	class SPH_EXPORT LocalListenerBehavior : public Behavior
	{
	public:
		LocalListenerBehavior(GameObject* parent, const std::string& name) : Behavior(parent, name) {}
		virtual ~LocalListenerBehavior() {
			for (auto it = subscribedColliders.begin(); it != subscribedColliders.end(); ++it) {
				(*it).second->UnSubscribeCollision(listenerID);
			}
		}

		inline void SubscribeCollision(Collider* collider, CollisionEventFunc&& func) {
			collider->SubscribeCollision(listenerID, std::move(func), SPH_BIND(LocalListenerBehavior::OnLocalSourceDestroyed));
			subscribedColliders[collider->GetID()] = collider;
		}

		inline void UnSubscribeCollision(Collider* collider) {
			collider->UnSubscribeCollision(listenerID);
			auto it = subscribedColliders.find(collider->GetID());
			if (it != subscribedColliders.end()) subscribedColliders.erase(it);
		}

		void OnLocalSourceDestroyed(const LocalSourceDestroyedEvent& e) {
			auto it = subscribedColliders.find(e.sourceID);
			if (it != subscribedColliders.end()) subscribedColliders.erase(it);
		}

	protected:
		std::unordered_map<ID, Collider*> subscribedColliders;
	};
}
#pragma once

#include "Engine/GameObjects/Colliders/Collider.hpp"
#include "Engine/CollisionSystem/CollisionEvent.hpp"


namespace Sharpheus {

	class ColliderListener
	{
	public:
		ColliderListener(ID listenerID) : listenerID(listenerID) {}
		virtual ~ColliderListener() {
			for (auto it = collisionSubscriptions.begin(); it != collisionSubscriptions.end(); ++it) {
				(*it).second->UnSubscribeCollision(listenerID);
				if (!IsSubscribedTo((*it).second->GetID())) {
					(*it).second->UnSubscribeForDestruction(listenerID);
				}
			}
			for (auto it = triggerEnterSubscriptions.begin(); it != triggerEnterSubscriptions.end(); ++it) {
				(*it).second->UnSubscribeTriggerEnter(listenerID);
				if (!IsSubscribedTo((*it).second->GetID())) {
					(*it).second->UnSubscribeForDestruction(listenerID);
				}
			}
			for (auto it = triggerExitSubscriptions.begin(); it != triggerExitSubscriptions.end(); ++it) {
				(*it).second->UnSubscribeTriggerExit(listenerID);
				(*it).second->UnSubscribeForDestruction(listenerID);
			}
		}

		inline void SubscribeCollision(Collider* collider, CollisionEventFunc&& func) {
			if (!IsSubscribedTo(collider->GetID())) {
				collider->SubscribeForDestruction(listenerID, SPH_BIND(ColliderListener::OnLocalSourceDestroyed));
			}
			collider->SubscribeCollision(listenerID, std::move(func));
			collisionSubscriptions[collider->GetID()] = collider;
		}

		inline void UnSubscribeCollision(Collider* collider) {
			collider->UnSubscribeCollision(listenerID);
			auto it = collisionSubscriptions.find(collider->GetID());
			if (it != collisionSubscriptions.end()) collisionSubscriptions.erase(it);

			if (!IsSubscribedTo(collider->GetID())) {
				collider->UnSubscribeForDestruction(listenerID);
			}
		}

		inline void SubscribeOnTriggerEnter(Collider* collider, OnEnterEventFunc&& func) {
			if (!IsSubscribedTo(collider->GetID())) {
				collider->SubscribeForDestruction(listenerID, SPH_BIND(ColliderListener::OnLocalSourceDestroyed));
			}
			collider->SubscribeTriggerEnter(listenerID, std::move(func));
			triggerEnterSubscriptions[collider->GetID()] = collider;
		}

		inline void UnSubscribeOnTriggerEnter(Collider* collider) {
			collider->UnSubscribeTriggerEnter(listenerID);
			auto it = triggerEnterSubscriptions.find(collider->GetID());
			if (it != triggerEnterSubscriptions.end()) triggerEnterSubscriptions.erase(it);

			if (!IsSubscribedTo(collider->GetID())) {
				collider->UnSubscribeForDestruction(listenerID);
			}
		}

		inline void SubscribeOnTriggerExit(Collider* collider, OnExitEventFunc&& func) {
			if (!IsSubscribedTo(collider->GetID())) {
				collider->SubscribeForDestruction(listenerID, SPH_BIND(ColliderListener::OnLocalSourceDestroyed));
			}

			collider->SubscribeTriggerExit(listenerID, std::move(func));
			triggerExitSubscriptions[collider->GetID()] = collider;
		}

		inline void UnSubscribeOnTriggerExit(Collider* collider) {
			collider->UnSubscribeTriggerExit(listenerID);
			auto it = triggerExitSubscriptions.find(collider->GetID());
			if (it != triggerExitSubscriptions.end()) triggerExitSubscriptions.erase(it);

			if (!IsSubscribedTo(collider->GetID())) {
				collider->UnSubscribeForDestruction(listenerID);
			}
		}

		void OnLocalSourceDestroyed(const GameObjectDestroyedEvent& e) {
			ID sourceID = e.source->GetID();
			auto it0 = collisionSubscriptions.find(sourceID);
			if (it0 != collisionSubscriptions.end()) collisionSubscriptions.erase(it0);
			auto it1 = triggerEnterSubscriptions.find(sourceID);
			if (it1 != triggerEnterSubscriptions.end()) triggerEnterSubscriptions.erase(it1);
			auto it2 = triggerExitSubscriptions.find(sourceID);
			if (it2 != triggerExitSubscriptions.end()) triggerExitSubscriptions.erase(it2);
		}

	protected:
		ID listenerID;
		std::unordered_map<ID, Collider*> collisionSubscriptions;
		std::unordered_map<ID, Collider*> triggerEnterSubscriptions;
		std::unordered_map<ID, Collider*> triggerExitSubscriptions;

		bool IsSubscribedTo(ID id) {
			auto it0 = collisionSubscriptions.find(id);
			if (it0 != collisionSubscriptions.end()) {
				return true;
			}

			auto it1 = triggerEnterSubscriptions.find(id);
			if (it1 != triggerEnterSubscriptions.end()) {
				return true;
			}

			auto it2 = triggerExitSubscriptions.find(id);
			if (it2 != triggerExitSubscriptions.end()) {
				return true;
			}

			return false;
		}
	};
}
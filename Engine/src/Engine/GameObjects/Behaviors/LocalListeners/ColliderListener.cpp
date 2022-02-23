#include "pch.h"
#include "ColliderListener.hpp"


namespace Sharpheus {

	ColliderListener::~ColliderListener()
	{
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

}
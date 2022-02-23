#pragma once

#include "../Controls/Control.hpp"


namespace Sharpheus {

	class SPH_EXPORT ControlListener
	{
	public:
		ControlListener(ID listenerID) : listenerID(listenerID) {}
		virtual ~ControlListener();

		inline void SubscribeControlChanged(Control* control, ControlChangedEventFunc&& func) {
			control->SubscribeForDestruction(listenerID, SPH_BIND(ControlListener::OnLocalSourceDestroyed));
			control->SubscribeForChange(listenerID, std::move(func));
			controlChangeSubscriptions[control->GetID()] = control;
		}

		inline void UnSubscribeControlChanged(Control* control) {
			control->UnSubscribeForChange(listenerID);
			auto it = controlChangeSubscriptions.find(control->GetID());
			if (it != controlChangeSubscriptions.end()) controlChangeSubscriptions.erase(it);
			control->UnSubscribeForDestruction(listenerID);
		}

		void OnLocalSourceDestroyed(const GameObjectDestroyedEvent& e) {
			ID sourceID = e.source->GetID();
			auto it = controlChangeSubscriptions.find(sourceID);
			if (it != controlChangeSubscriptions.end()) controlChangeSubscriptions.erase(it);
		}

	protected:
		ID listenerID;
		std::unordered_map<ID, Control*> controlChangeSubscriptions;
	};

}
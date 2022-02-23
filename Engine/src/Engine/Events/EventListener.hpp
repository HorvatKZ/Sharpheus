#pragma once

#include "EventHandler.hpp"


namespace Sharpheus {

	class SPH_EXPORT EventListener
	{
	public:
		EventListener();
		virtual ~EventListener();
		
		EventListener(const EventListener& other);

		inline ID GetID() { return listenerID; }
		

		template <class T_Event>
		void Subscribe(std::function<void(const T_Event&)>&& func) {
			EventHandler::Subscribe<T_Event>(listenerID, std::move(func));
			++subscribedNum;
		}

		template <class T_Event>
		void UnSubscribe() {
			EventHandler::UnSubscribe<T_Event>(listenerID);
			--subscribedNum;
		}

		void UnSubscribeAll() {
			EventHandler::UnSubscribeAll(listenerID);
			subscribedNum = 0;
		}

	protected:
		static ID nextFreeID;

		uint32 subscribedNum;
		ID listenerID;
	};
}
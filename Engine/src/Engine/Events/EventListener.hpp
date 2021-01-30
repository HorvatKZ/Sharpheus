#pragma once

#include "pch.h"
#include "EventHandler.hpp"


namespace Sharpheus {

	class EventListener
	{
	public:
		EventListener();
		virtual ~EventListener();
		
		EventListener(EventListener& other);
		EventListener& operator=(EventListener& other);
		EventListener(EventListener&& other) noexcept;
		EventListener& operator=(EventListener&& other) noexcept;
		

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
		static uint32_t nextFreeID;

		uint32_t subscribedNum;
		uint32_t listenerID;
	};
}
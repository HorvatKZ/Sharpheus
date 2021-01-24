#pragma once

#include "pch.h"
#include "EventHandler.hpp"


namespace Sharpheus {

	class EventListener
	{
	public:
		EventListener() : listenerID(nextFreeID++), subscribedNum(0) {}
		virtual ~EventListener() {
			if (subscribedNum != 0) {
				EventHandler::UnSubscribeAll(listenerID);
			}
		}
		
		EventListener(EventListener& /*other*/) : listenerID(nextFreeID++), subscribedNum(0) {}
		EventListener& operator=(EventListener& other) { *this = other; return *this; }
		EventListener(EventListener&& other) noexcept : listenerID(other.listenerID) { other.listenerID = 0; }
		EventListener& operator=(EventListener&& other) noexcept { *this = std::move(other); return *this; }
		

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

	uint32_t EventListener::nextFreeID = 1;
}
#include "pch.h"
#include "EventListener.hpp"


namespace Sharpheus {

	ID EventListener::nextFreeID = 1;

	EventListener::EventListener() : listenerID(nextFreeID++), subscribedNum(0) {}


	EventListener::~EventListener()
	{
		if (subscribedNum != 0) {
			EventHandler::UnSubscribeAll(listenerID);
		}
	}

	EventListener::EventListener(EventListener& /*other*/) : listenerID(nextFreeID++), subscribedNum(0) {}
	EventListener& EventListener::operator=(EventListener& other) { *this = other; return *this; }
	EventListener::EventListener(EventListener&& other) noexcept : listenerID(other.listenerID) { other.listenerID = 0; }
	EventListener& EventListener::operator=(EventListener&& other) noexcept { *this = std::move(other); return *this; }

}
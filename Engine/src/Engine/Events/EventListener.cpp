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

	EventListener::EventListener(const EventListener& /*other*/) : listenerID(nextFreeID++), subscribedNum(0) {}

}
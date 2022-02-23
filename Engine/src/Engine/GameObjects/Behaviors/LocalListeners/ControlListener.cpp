#include "pch.h"
#include "ControlListener.hpp"


namespace Sharpheus {

	ControlListener::~ControlListener()
	{
		for (auto it = controlChangeSubscriptions.begin(); it != controlChangeSubscriptions.end(); ++it) {
			(*it).second->UnSubscribeForChange(listenerID);
			(*it).second->UnSubscribeForDestruction(listenerID);
		}
	}

}
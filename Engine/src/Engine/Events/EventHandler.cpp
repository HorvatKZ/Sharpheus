#include "pch.h"
#include "EventHandler.hpp"


namespace Sharpheus {

	std::unordered_map<uint32_t, WindowClosedEventFunc> EventHandler::windowClosedListeners;
	std::unordered_map<uint32_t, WindowResizedEventFunc> EventHandler::windowResizedListeners;
	WindowClosedEventFunc EventHandler::closeGame;


	void EventHandler::Init(const WindowClosedEventFunc& _closeGame)
	{
		closeGame = _closeGame;
	}


	void EventHandler::Clear()
	{

	}

	
	void EventHandler::Handle(const Event& e)
	{
		SPH_LOG("{0} catched", e.ToStr());

		switch (e.GetType()) {
			case Event::Type::WindowClosed:
				HandleWindowsClosed(static_cast<const WindowClosedEvent&>(e));
				break;
			case Event::Type::WindowResized:
				HandleWindowsResized(static_cast<const WindowResizedEvent&>(e));
				break;
			default:
				SPH_WARN("Uknown typed event catched: {0}", e.GetType());
		}
	}


	void EventHandler::UnSubscribeAll(uint32_t listenerID)
	{
		SPH_UNSUBSCRIBE_EVENTTYPE(windowClosedListeners);
		SPH_UNSUBSCRIBE_EVENTTYPE(windowResizedListeners);
	}


	void EventHandler::HandleWindowsClosed(const WindowClosedEvent& e)
	{
		for (auto it = windowClosedListeners.begin(); it != windowClosedListeners.end(); ++it) {
			(*it).second(e);
		}
		closeGame(e);
	}

	void EventHandler::HandleWindowsResized(const WindowResizedEvent& e)
	{
		for (auto it = windowResizedListeners.begin(); it != windowResizedListeners.end(); ++it) {
			(*it).second(e);
		}
	}
}
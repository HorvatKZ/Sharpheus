#include "pch.h"
#include "EventHandler.hpp"


namespace Sharpheus {

	std::unordered_map<uint32_t, WindowClosedEventFunc> EventHandler::windowClosedListeners;
	std::unordered_map<uint32_t, WindowResizedEventFunc> EventHandler::windowResizedListeners;
	std::unordered_map<uint32_t, KeyPressedEventFunc> EventHandler::keyPressedListeners;
	std::unordered_map<uint32_t, KeyReleasedEventFunc> EventHandler::keyReleasedListeners;
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
		switch (e.GetType()) {
			case Event::Type::WindowClosed:
				HandleWindowsClosed(static_cast<const WindowClosedEvent&>(e));
				break;
			case Event::Type::WindowResized:
				HandleWindowsResized(static_cast<const WindowResizedEvent&>(e));
				break;
			case Event::Type::KeyPressed:
				HandleKeyPressed(static_cast<const KeyPressedEvent&>(e));
				break;
			case Event::Type::KeyReleased:
				HandleKeyReleased(static_cast<const KeyReleasedEvent&>(e));
				break;
			default:
				SPH_WARN("Uknown typed event catched: {0}", e.ToStr());
		}
	}


	void EventHandler::UnSubscribeAll(uint32_t listenerID)
	{
		SPH_UNSUBSCRIBE_EVENTS_IN(windowClosedListeners);
		SPH_UNSUBSCRIBE_EVENTS_IN(windowResizedListeners);
		SPH_UNSUBSCRIBE_EVENTS_IN(keyPressedListeners);
		SPH_UNSUBSCRIBE_EVENTS_IN(keyReleasedListeners);
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

	void EventHandler::HandleKeyPressed(const KeyPressedEvent& e)
	{
		for (auto it = keyPressedListeners.begin(); it != keyPressedListeners.end(); ++it) {
			(*it).second(e);
		}
	}

	void EventHandler::HandleKeyReleased(const KeyReleasedEvent& e)
	{
		for (auto it = keyReleasedListeners.begin(); it != keyReleasedListeners.end(); ++it) {
			(*it).second(e);
		}
	}
}
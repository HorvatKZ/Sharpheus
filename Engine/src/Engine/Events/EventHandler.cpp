#include "pch.h"
#include "EventHandler.hpp"

namespace Sharpheus {

	std::unordered_map<ID, WindowClosedEventFunc> EventHandler::windowClosedListeners;
	std::unordered_map<ID, WindowResizedEventFunc> EventHandler::windowResizedListeners;
	std::unordered_map<ID, KeyPressedEventFunc> EventHandler::keyPressedListeners;
	std::unordered_map<ID, KeyRepeatEventFunc> EventHandler::keyRepeatListeners;
	std::unordered_map<ID, KeyReleasedEventFunc> EventHandler::keyReleasedListeners;
	std::unordered_map<ID, MousePressedEventFunc> EventHandler::mousePressedListeners;
	std::unordered_map<ID, MouseReleasedEventFunc> EventHandler::mouseReleasedListeners;
	std::unordered_map<ID, MouseMovedEventFunc> EventHandler::mouseMovedListeners;
	std::unordered_map<ID, MouseScrolledEventFunc> EventHandler::mouseScrolledListeners;
	WindowClosedEventFunc EventHandler::closeGame;


	void EventHandler::Init(const WindowClosedEventFunc& _closeGame)
	{
		Clear();

		closeGame = _closeGame;
	}


	void EventHandler::Clear()
	{
		windowClosedListeners.clear();
		windowResizedListeners.clear();
		keyPressedListeners.clear();
		keyRepeatListeners.clear();
		keyReleasedListeners.clear();
		mousePressedListeners.clear();
		mouseReleasedListeners.clear();
		mouseMovedListeners.clear();
		mouseScrolledListeners.clear();
	}

	
	void EventHandler::Handle(const Event& e)
	{
		switch (e.GetType()) {
			case Event::Type::Local:
				SPH_WARN("Local event caught in global EventHanler. Event ignored");
				break;
			case Event::Type::WindowClosed:
				HandleWindowsClosed(static_cast<const WindowClosedEvent&>(e));
				break;
			case Event::Type::WindowResized:
				HandleWindowsResized(static_cast<const WindowResizedEvent&>(e));
				break;
			case Event::Type::KeyPressed:
				HandleKeyPressed(static_cast<const KeyPressedEvent&>(e));
				break;
			case Event::Type::KeyRepeat:
				HandleKeyRepeat(static_cast<const KeyRepeatEvent&>(e));
				break;
			case Event::Type::KeyReleased:
				HandleKeyReleased(static_cast<const KeyReleasedEvent&>(e));
				break;
			case Event::Type::MousePressed:
				HandleMousePressed(static_cast<const MousePressedEvent&>(e));
				break;
			case Event::Type::MouseReleased:
				HandleMouseReleased(static_cast<const MouseReleasedEvent&>(e));
				break;
			case Event::Type::MouseMoved:
				HandleMouseMoved(static_cast<const MouseMovedEvent&>(e));
				break;
			case Event::Type::MouseScrolled:
				HandleMouseScrolled(static_cast<const MouseScrolledEvent&>(e));
				break;
			default:
				SPH_WARN("Uknown typed event catched: {0}", e.ToStr());
		}
	}


	void EventHandler::UnSubscribeAll(ID listenerID)
	{
		SPH_UNSUBSCRIBE_EVENTS_IN(windowClosedListeners);
		SPH_UNSUBSCRIBE_EVENTS_IN(windowResizedListeners);
		SPH_UNSUBSCRIBE_EVENTS_IN(keyPressedListeners);
		SPH_UNSUBSCRIBE_EVENTS_IN(keyRepeatListeners);
		SPH_UNSUBSCRIBE_EVENTS_IN(keyReleasedListeners);
		SPH_UNSUBSCRIBE_EVENTS_IN(mousePressedListeners);
		SPH_UNSUBSCRIBE_EVENTS_IN(mouseReleasedListeners);
		SPH_UNSUBSCRIBE_EVENTS_IN(mouseMovedListeners);
		SPH_UNSUBSCRIBE_EVENTS_IN(mouseScrolledListeners);
	}


	void EventHandler::HandleWindowsClosed(const WindowClosedEvent& e)
	{
		SPH_NOTIFY_LISTENERS(windowClosedListeners);
		closeGame(e);
	}

	void EventHandler::HandleWindowsResized(const WindowResizedEvent& e)
	{
		SPH_NOTIFY_LISTENERS(windowResizedListeners);
	}

	void EventHandler::HandleKeyPressed(const KeyPressedEvent& e)
	{
		SPH_NOTIFY_LISTENERS(keyPressedListeners);
	}

	void EventHandler::HandleKeyRepeat(const KeyRepeatEvent& e)
	{
		SPH_NOTIFY_LISTENERS(keyRepeatListeners);
	}

	void EventHandler::HandleKeyReleased(const KeyReleasedEvent& e)
	{
		SPH_NOTIFY_LISTENERS(keyReleasedListeners);
	}

	void EventHandler::HandleMousePressed(const MousePressedEvent& e)
	{
		SPH_NOTIFY_LISTENERS(mousePressedListeners);
	}

	void EventHandler::HandleMouseReleased(const MouseReleasedEvent& e)
	{
		SPH_NOTIFY_LISTENERS(mouseReleasedListeners);
	}

	void EventHandler::HandleMouseMoved(const MouseMovedEvent& e)
	{
		SPH_NOTIFY_LISTENERS(mouseMovedListeners);
	}

	void EventHandler::HandleMouseScrolled(const MouseScrolledEvent& e)
	{
		SPH_NOTIFY_LISTENERS(mouseScrolledListeners);
	}

}
#pragma once

#include "WindowEvents.hpp"
#include "KeyEvents.hpp"

#define SPH_DECL_EVENTTYPE_SUBSCRIPTION(eventClass, listeners) template <> \
	inline static void Subscribe<eventClass>(ID listenerID, eventClass##Func&& func) { \
		listeners[listenerID] = func; \
	}

#define SPH_UNSUBSCRIBE_EVENTS_IN(listeners) { \
		auto it = listeners.find(listenerID); \
		if (it != listeners.end()) listeners.erase(it); \
	}


namespace Sharpheus {

	typedef uint32_t ID;


	class SPH_EXPORT EventHandler
	{
	public:
		static void Init(const WindowClosedEventFunc& _closeGame);
		static void Clear();

		static void Handle(const Event& e);

		template <class T_Event>
		inline static void Subscribe(ID listenerID, std::function<void(const T_Event&)>&& func) {
			SPH_WARN("Uknown typed event tries to subscribe: {0}. Subscription request ignored!", T_Event::GetStaticType());
		}

		SPH_DECL_EVENTTYPE_SUBSCRIPTION(WindowClosedEvent, windowClosedListeners)
		SPH_DECL_EVENTTYPE_SUBSCRIPTION(WindowResizedEvent, windowResizedListeners)
		SPH_DECL_EVENTTYPE_SUBSCRIPTION(KeyPressedEvent, keyPressedListeners)
		SPH_DECL_EVENTTYPE_SUBSCRIPTION(KeyRepeatEvent, keyRepeatListeners)
		SPH_DECL_EVENTTYPE_SUBSCRIPTION(KeyReleasedEvent, keyReleasedListeners)


		template <class T_Event>
		inline static void UnSubscribe(ID listenerID) {
			switch (T_Event::GetStaticType()) {
			case Event::Type::WindowClosed:
				SPH_UNSUBSCRIBE_EVENTS_IN(windowClosedListeners);
				break;
			case Event::Type::WindowResized:
				SPH_UNSUBSCRIBE_EVENTS_IN(windowResizedListeners);
				break;
			case Event::Type::KeyPressed:
				SPH_UNSUBSCRIBE_EVENTS_IN(keyPressedListeners);
				break;
			case Event::Type::KeyRepeat:
				SPH_UNSUBSCRIBE_EVENTS_IN(keyRepeatListeners);
				break;
			case Event::Type::KeyReleased:
				SPH_UNSUBSCRIBE_EVENTS_IN(keyReleasedListeners);
				break;
			default:
				SPH_WARN("Uknown typed event tries to unsubscribe: {0}. Unsubscription request ignored!", T_Event::GetStaticType());
			}
		}

		static void UnSubscribeAll(ID listenerID);

	private:
		static std::unordered_map<ID, WindowClosedEventFunc> windowClosedListeners;
		static std::unordered_map<ID, WindowResizedEventFunc> windowResizedListeners;
		static std::unordered_map<ID, KeyPressedEventFunc> keyPressedListeners;
		static std::unordered_map<ID, KeyRepeatEventFunc> keyRepeatListeners;
		static std::unordered_map<ID, KeyReleasedEventFunc> keyReleasedListeners;
		static WindowClosedEventFunc closeGame;

		static void HandleWindowsClosed(const WindowClosedEvent& e);
		static void HandleWindowsResized(const WindowResizedEvent& e);
		static void HandleKeyPressed(const KeyPressedEvent& e);
		static void HandleKeyRepeat(const KeyRepeatEvent& e);
		static void HandleKeyReleased(const KeyReleasedEvent& e);
	};

}
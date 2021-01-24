#pragma once

#include "pch.h"
#include "WindowEvents.hpp"

#define SPH_DECL_EVENTTYPE_SUBSCRIPTION(eventClass, listeners) template <> \
	inline static void Subscribe<eventClass>(uint32_t listenerID, eventClass##Func&& func) { \
		listeners[listenerID] = func; \
	}

#define SPH_UNSUBSCRIBE_EVENTTYPE(listeners) { \
		auto it = listeners.find(listenerID); \
		if (it != listeners.end()) listeners.erase(it); \
	}


namespace Sharpheus {

	class SPH_EXPORT EventHandler
	{
	public:
		static void Init(const WindowClosedEventFunc& _closeGame);
		static void Clear();

		static void Handle(const Event& e);

		template <class T_Event>
		inline static void Subscribe(uint32_t listenerID, std::function<void(const T_Event&)>&& func) {
			SPH_WARN("Uknown typed event tries to subscribe: {0}. Subscription request ignored!", T_Event::GetStaticType());
		}

		SPH_DECL_EVENTTYPE_SUBSCRIPTION(WindowClosedEvent, windowClosedListeners)
		SPH_DECL_EVENTTYPE_SUBSCRIPTION(WindowResizedEvent, windowResizedListeners)


		template <class T_Event>
		inline static void UnSubscribe(uint32_t listenerID) {
			switch (T_Event::GetStaticType()) {
			case Event::Type::WindowClosed:
				SPH_UNSUBSCRIBE_EVENTTYPE(windowClosedListeners);
				break;
			case Event::Type::WindowResized:
				SPH_UNSUBSCRIBE_EVENTTYPE(windowResizedListeners);
				break;
			default:
				SPH_WARN("Uknown typed event tries to unsubscribe: {0}. Unsubscription request ignored!", T_Event::GetStaticType());
			}
		}

		static void UnSubscribeAll(uint32_t listenerID);

	private:
		static std::unordered_map<uint32_t, WindowClosedEventFunc> windowClosedListeners;
		static std::unordered_map<uint32_t, WindowResizedEventFunc> windowResizedListeners;
		static WindowClosedEventFunc closeGame;

		static void HandleWindowsClosed(const WindowClosedEvent& e);
		static void HandleWindowsResized(const WindowResizedEvent& e);
	};

}
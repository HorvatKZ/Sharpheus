#pragma once

#include "Event.hpp"


namespace Sharpheus {

	typedef uint32_t ID;

	SPH_DEF_EVENT_FUNC(LocalSourceDestroyedEvent)


	template<class T>
	class SPH_EXPORT LocalEvent : public Event
	{
	public:
		LocalEvent(T* source) : source(source) {}
		virtual ~LocalEvent() = default;

		SPH_DECL_EVENT(Local)

		T* source;
	};


	class SPH_EXPORT LocalSourceDestroyedEvent : public Event
	{
	public:
		LocalSourceDestroyedEvent(ID sourceID) : sourceID(sourceID) {}
		virtual ~LocalSourceDestroyedEvent() = default;

		inline std::string ToStr() const override { return "LocalSourceDestroyedEvent " + std::to_string(sourceID); }

		SPH_DECL_EVENT(Local)

		ID sourceID;
	};
}
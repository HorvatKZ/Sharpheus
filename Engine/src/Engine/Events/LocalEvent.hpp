#pragma once

#include "Event.hpp"


namespace Sharpheus {

	template<class T>
	class SPH_EXPORT LocalEvent : public Event
	{
	public:
		LocalEvent(T* source) : source(source) {}
		virtual ~LocalEvent() = default;

		SPH_DECL_EVENT(Local)

		T* source;
	};
}
#pragma once

#include "pch.h"
#include "Event.hpp"


namespace Sharpheus {

	SPH_DEF_EVENT_FUNC(WindowClosedEvent)
	SPH_DEF_EVENT_FUNC(WindowResizedEvent)


	class SPH_EXPORT WindowClosedEvent : public Event
	{
	public:
		WindowClosedEvent() = default;
		virtual ~WindowClosedEvent() = default;

		inline std::string ToStr() const override { return "WindowClosedEvent"; }

		SPH_DECL_EVENT(WindowClosed)
	};


	class SPH_EXPORT WindowResizedEvent : public Event
	{
	public:
		WindowResizedEvent(uint32_t newWidth, uint32_t newHeight) : newWidth(newWidth), newHeight(newHeight) {}
		virtual ~WindowResizedEvent() = default;

		inline std::string ToStr() const override { return "WindowResizedEvent " + std::to_string(newWidth) + "x" + std::to_string(newHeight); }

		SPH_DECL_EVENT(WindowResized)
	
		uint32_t newWidth, newHeight;
	};
}
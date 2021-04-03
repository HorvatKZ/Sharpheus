#pragma once

#include "Event.hpp"


namespace Sharpheus {

	SPH_DEF_EVENT_FUNC(MousePressedEvent)
	SPH_DEF_EVENT_FUNC(MouseReleasedEvent)
	SPH_DEF_EVENT_FUNC(MouseMovedEvent)
	SPH_DEF_EVENT_FUNC(MouseScrolledEvent)


	enum class MouseButton {
		// Main
		LEFT = 1, RIGHT = 2, MIDDLE = 3,

		// Other
		BUTTON_4 = 4, BUTTON_5 = 5, BUTTON_6 = 6, BUTTON_7 = 7, BUTTON_8 = 8
	};


	class SPH_EXPORT MousePressedEvent : public Event
	{
	public:
		MousePressedEvent(const Point& screenPos, const Point& gamePos, MouseButton button)
			: screenPos(screenPos), gamePos(gamePos), button(button) {}
		virtual ~MousePressedEvent() = default;

		inline std::string ToStr() const override {
			return "MousePressedEvent ScreenPos: (" + std::to_string(screenPos.x) + ", " + std::to_string(screenPos.y) +
				"), GamePos: (" + std::to_string(gamePos.x) + ", " + std::to_string(gamePos.y) + "), Button: " + std::to_string((int)button);
		}

		SPH_DECL_EVENT(MousePressed);

		Point screenPos;
		Point gamePos;
		MouseButton button;
	};


	class SPH_EXPORT MouseReleasedEvent : public Event
	{
	public:
		MouseReleasedEvent(const Point& screenPos, const Point& gamePos, MouseButton button)
			: screenPos(screenPos), gamePos(gamePos), button(button) {}
		virtual ~MouseReleasedEvent() = default;

		inline std::string ToStr() const override {
			return "MouseReleasedEvent ScreenPos: (" + std::to_string(screenPos.x) + ", " + std::to_string(screenPos.y) +
				"), GamePos: (" + std::to_string(gamePos.x) + ", " + std::to_string(gamePos.y) + "), Button: " + std::to_string((int)button);
		}

		SPH_DECL_EVENT(MouseReleased);

		Point screenPos;
		Point gamePos;
		MouseButton button;
	};


	class SPH_EXPORT MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(const Point& screenPos, const Point& gamePos)
			: screenPos(screenPos), gamePos(gamePos) {}
		virtual ~MouseMovedEvent() = default;

		inline std::string ToStr() const override {
			return "MouseMovedEvent ScreenPos: (" + std::to_string(screenPos.x) + ", " + std::to_string(screenPos.y) +
				"), GamePos: (" + std::to_string(gamePos.x) + ", " + std::to_string(gamePos.y) + ")";
		}

		SPH_DECL_EVENT(MouseMoved);

		Point screenPos;
		Point gamePos;
	};


	class SPH_EXPORT MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(const Point& screenPos, const Point& gamePos, const Point& offset)
			: screenPos(screenPos), gamePos(gamePos), offset(offset) {}
		virtual ~MouseScrolledEvent() = default;

		inline std::string ToStr() const override {
			return "MouseScrolledEvent ScreenPos: (" + std::to_string(screenPos.x) + ", " + std::to_string(screenPos.y) +
				"), GamePos: (" + std::to_string(gamePos.x) + ", " + std::to_string(gamePos.y) + "), Offset: (" +
				std::to_string(offset.x) + ", " + std::to_string(offset.y) + ")";
		}

		SPH_DECL_EVENT(MouseScrolled);

		Point screenPos;
		Point gamePos;
		Point offset;
	};

}
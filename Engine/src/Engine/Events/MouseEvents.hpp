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


	class SPH_EXPORT MouseEvent : public Event
	{
	public:
		MouseEvent(const Point& screenPos, const Point& gamePos)
			: screenPos(screenPos), gamePos(gamePos) {}
		virtual ~MouseEvent() = default;

		Point screenPos;
		Point gamePos;
	};


	class SPH_EXPORT MousePressedEvent : public MouseEvent
	{
	public:
		MousePressedEvent(const Point& screenPos, const Point& gamePos, MouseButton button)
			: MouseEvent(screenPos, gamePos), button(button) {}
		virtual ~MousePressedEvent() = default;

		inline std::string ToStr() const override {
			return "MousePressedEvent ScreenPos: (" + std::to_string(screenPos.x) + ", " + std::to_string(screenPos.y) +
				"), GamePos: (" + std::to_string(gamePos.x) + ", " + std::to_string(gamePos.y) + "), Button: " + std::to_string((int)button);
		}

		SPH_DECL_EVENT(MousePressed);

		MouseButton button;
	};


	class SPH_EXPORT MouseReleasedEvent : public MouseEvent
	{
	public:
		MouseReleasedEvent(const Point& screenPos, const Point& gamePos, MouseButton button)
			: MouseEvent(screenPos, gamePos), button(button) {}
		virtual ~MouseReleasedEvent() = default;

		inline std::string ToStr() const override {
			return "MouseReleasedEvent ScreenPos: (" + std::to_string(screenPos.x) + ", " + std::to_string(screenPos.y) +
				"), GamePos: (" + std::to_string(gamePos.x) + ", " + std::to_string(gamePos.y) + "), Button: " + std::to_string((int)button);
		}

		SPH_DECL_EVENT(MouseReleased);

		MouseButton button;
	};


	class SPH_EXPORT MouseMovedEvent : public MouseEvent
	{
	public:
		MouseMovedEvent(const Point& screenPos, const Point& gamePos)
			: MouseEvent(screenPos, gamePos) {}
		virtual ~MouseMovedEvent() = default;

		inline std::string ToStr() const override {
			return "MouseMovedEvent ScreenPos: (" + std::to_string(screenPos.x) + ", " + std::to_string(screenPos.y) +
				"), GamePos: (" + std::to_string(gamePos.x) + ", " + std::to_string(gamePos.y) + ")";
		}

		SPH_DECL_EVENT(MouseMoved);
	};


	class SPH_EXPORT MouseScrolledEvent : public MouseEvent
	{
	public:
		MouseScrolledEvent(const Point& screenPos, const Point& gamePos, bool isUp)
			: MouseEvent(screenPos, gamePos), isUp(isUp) {}
		virtual ~MouseScrolledEvent() = default;

		inline std::string ToStr() const override {
			return "MouseScrolledEvent ScreenPos: (" + std::to_string(screenPos.x) + ", " + std::to_string(screenPos.y) +
				"), GamePos: (" + std::to_string(gamePos.x) + ", " + std::to_string(gamePos.y) + "), Dierction: " + std::to_string(isUp);
		}

		SPH_DECL_EVENT(MouseScrolled);

		bool isUp;
	};

}
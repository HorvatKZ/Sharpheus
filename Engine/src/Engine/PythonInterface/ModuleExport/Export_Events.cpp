#include "pch.h"
#include "PydExport.hpp"
#include "Engine/Events/EventListener.hpp"
#include "Engine/Events/EventHandler.hpp"


namespace Sharpheus {

	void Export_Event(py::module_& handle)
	{
		py::class_<Event> evnt(handle, "Event");

		py::enum_<Event::Type>(evnt, "Type")
			.value("None", Event::Type::None)
			.value("Local", Event::Type::Local)
			.value("WindowClosed", Event::Type::WindowClosed)
			.value("WindowResized", Event::Type::WindowResized)
			.value("KeyPressed", Event::Type::KeyPressed)
			.value("KeyHold", Event::Type::KeyHold)
			.value("KeyReleased", Event::Type::KeyReleased)
			.value("MousePressed", Event::Type::MousePressed)
			.value("MouseReleased", Event::Type::MouseReleased)
			.value("MouseMoved", Event::Type::MouseMoved)
			.value("MouseScrolled", Event::Type::MouseScrolled);

		evnt.def_static("static_type", &Event::GetStaticType)
			.def("type", &Event::GetType)
			.def("__str__", &Event::ToStr);
	}


	void Export_KeyEvents(py::module_& handle)
	{
		py::enum_<KeyCode>(handle, "KeyCode")
			.value("UNKNOWN", KeyCode::UNKNOWN)

			.value("RIGHT", KeyCode::RIGHT)
			.value("LEFT", KeyCode::LEFT)
			.value("DOWN", KeyCode::DOWN)
			.value("UP", KeyCode::UP)

			.value("A", KeyCode::A)
			.value("B", KeyCode::B)
			.value("C", KeyCode::C)
			.value("D", KeyCode::D)
			.value("E", KeyCode::E)
			.value("F", KeyCode::F)
			.value("G", KeyCode::G)
			.value("H", KeyCode::H)
			.value("I", KeyCode::I)
			.value("J", KeyCode::J)
			.value("K", KeyCode::K)
			.value("L", KeyCode::L)
			.value("M", KeyCode::M)
			.value("N", KeyCode::N)
			.value("O", KeyCode::O)
			.value("P", KeyCode::P)
			.value("Q", KeyCode::Q)
			.value("R", KeyCode::R)
			.value("S", KeyCode::S)
			.value("T", KeyCode::T)
			.value("U", KeyCode::U)
			.value("V", KeyCode::V)
			.value("W", KeyCode::W)
			.value("X", KeyCode::X)
			.value("Y", KeyCode::Y)
			.value("Z", KeyCode::Z)

			.value("SPACE", KeyCode::SPACE)
			.value("APOSTROPHE", KeyCode::APOSTROPHE)
			.value("COMMA", KeyCode::COMMA)
			.value("MINUS", KeyCode::MINUS)
			.value("PERIOD", KeyCode::PERIOD)
			.value("SLASH", KeyCode::SLASH)
			.value("SEMICOLON", KeyCode::SEMICOLON)
			.value("EQUAL", KeyCode::EQUAL)
			.value("LEFT_BRACKET", KeyCode::LEFT_BRACKET)
			.value("BACKSLASH", KeyCode::BACKSLASH)
			.value("RIGHT_BRACKET", KeyCode::RIGHT_BRACKET)
			.value("BACKTICK", KeyCode::BACKTICK)

			.value("KEY_0", KeyCode::KEY_0)
			.value("KEY_1", KeyCode::KEY_1)
			.value("KEY_2", KeyCode::KEY_2)
			.value("KEY_3", KeyCode::KEY_3)
			.value("KEY_4", KeyCode::KEY_4)
			.value("KEY_5", KeyCode::KEY_5)
			.value("KEY_6", KeyCode::KEY_6)
			.value("KEY_7", KeyCode::KEY_7)
			.value("KEY_8", KeyCode::KEY_8)
			.value("KEY_9", KeyCode::KEY_9)

			.value("NP_0", KeyCode::NP_0)
			.value("NP_1", KeyCode::NP_1)
			.value("NP_2", KeyCode::NP_2)
			.value("NP_3", KeyCode::NP_3)
			.value("NP_4", KeyCode::NP_4)
			.value("NP_5", KeyCode::NP_5)
			.value("NP_6", KeyCode::NP_6)
			.value("NP_7", KeyCode::NP_7)
			.value("NP_8", KeyCode::NP_8)
			.value("NP_9", KeyCode::NP_9)
			.value("NP_9", KeyCode::NP_9)
			.value("NP_DECIMAL", KeyCode::NP_DECIMAL)
			.value("NP_DIVIDE", KeyCode::NP_DIVIDE)
			.value("NP_MULTIPLY", KeyCode::NP_MULTIPLY)
			.value("NP_SUBTRACT", KeyCode::NP_SUBTRACT)
			.value("NP_ADD", KeyCode::NP_ADD)
			.value("NP_ENTER", KeyCode::NP_ENTER)
			.value("NP_EQUAL", KeyCode::NP_EQUAL)

			.value("ESCAPE", KeyCode::ESCAPE)
			.value("ENTER", KeyCode::ENTER)
			.value("TAB", KeyCode::TAB)
			.value("BACKSPACE", KeyCode::BACKSPACE)
			.value("INSERT", KeyCode::INSERT)
			.value("DEL", KeyCode::DEL)
			.value("PAGE_UP", KeyCode::PAGE_UP)
			.value("PAGE_DOWN", KeyCode::PAGE_DOWN)
			.value("HOME", KeyCode::HOME)
			.value("END", KeyCode::END)
			.value("CAPS_LOCK", KeyCode::CAPS_LOCK)
			.value("SCROLL_LOCK", KeyCode::SCROLL_LOCK)
			.value("NUM_LOCK", KeyCode::NUM_LOCK)
			.value("PRINT_SCREEN", KeyCode::NP_EQUAL)
			.value("PAUSE", KeyCode::PAUSE)
			.value("LEFT_SHIFT", KeyCode::LEFT_SHIFT)
			.value("LEFT_CONTROL", KeyCode::LEFT_CONTROL)
			.value("LEFT_ALT", KeyCode::LEFT_ALT)
			.value("LEFT_SUPER", KeyCode::LEFT_SUPER)
			.value("RIGHT_SHIFT", KeyCode::RIGHT_SHIFT)
			.value("RIGHT_CONTROL", KeyCode::RIGHT_CONTROL)
			.value("RIGHT_ALT", KeyCode::RIGHT_ALT)
			.value("RIGHT_SUPER", KeyCode::RIGHT_SUPER)
			.value("MENU", KeyCode::MENU)

			.value("F1", KeyCode::F1)
			.value("F2", KeyCode::F2)
			.value("F3", KeyCode::F3)
			.value("F4", KeyCode::F4)
			.value("F5", KeyCode::F5)
			.value("F6", KeyCode::F6)
			.value("F7", KeyCode::F7)
			.value("F8", KeyCode::F8)
			.value("F9", KeyCode::F9)
			.value("F10", KeyCode::F10)
			.value("F11", KeyCode::F11)
			.value("F12", KeyCode::F12)
			.value("F13", KeyCode::F13)
			.value("F14", KeyCode::F14)
			.value("F15", KeyCode::F15)
			.value("F16", KeyCode::F16)
			.value("F17", KeyCode::F17)
			.value("F18", KeyCode::F18)
			.value("F19", KeyCode::F19)
			.value("F20", KeyCode::F20)
			.value("F21", KeyCode::F21)
			.value("F22", KeyCode::F22)
			.value("F23", KeyCode::F23)
			.value("F24", KeyCode::F24)
			.value("F25", KeyCode::F25)

			.value("WORLD_1", KeyCode::WORLD_1)
			.value("WORLD_2", KeyCode::WORLD_2);

		py::class_<KeyEvent, Event>(handle, "KeyEvent")
			.def_readonly("code", &KeyEvent::code);
		
		py::class_<KeyPressedEvent, KeyEvent>(handle, "KeyPressedEvent")
			.def(py::init<KeyCode>(), "code"_a);

		py::class_<KeyHoldEvent, KeyEvent>(handle, "KeyHoldEvent")
			.def(py::init<KeyCode>(), "code"_a);

		py::class_<KeyReleasedEvent, KeyEvent>(handle, "KeyReleasedEvent")
			.def(py::init<KeyCode>(), "code"_a);
	}


	void Export_MouseEvents(py::module_& handle)
	{
		py::enum_<MouseButton>(handle, "MouseButton")
			.value("LEFT", MouseButton::LEFT)
			.value("RIGHT", MouseButton::RIGHT)
			.value("MIDDLE", MouseButton::MIDDLE)

			.value("BUTTON_4", MouseButton::BUTTON_4)
			.value("BUTTON_5", MouseButton::BUTTON_5)
			.value("BUTTON_6", MouseButton::BUTTON_6)
			.value("BUTTON_7", MouseButton::BUTTON_7)
			.value("BUTTON_8", MouseButton::BUTTON_8);

		py::class_<MouseEvent, Event>(handle, "MouseEvent")
			.def_readonly("screen_pos", &MouseEvent::screenPos)
			.def_readonly("game_pos", &MouseEvent::gamePos);

		py::class_<MousePressedEvent, MouseEvent>(handle, "MousePressedEvent")
			.def(py::init<const Point&, const Point&, MouseButton>(), "screen_pos"_a, "game_pos"_a, "button"_a)
			.def_readonly("button", &MousePressedEvent::button);

		py::class_<MouseReleasedEvent, MouseEvent>(handle, "MouseReleasedEvent")
			.def(py::init<const Point&, const Point&, MouseButton>(), "screen_pos"_a, "game_pos"_a, "button"_a)
			.def_readonly("button", &MouseReleasedEvent::button);

		py::class_<MouseMovedEvent, MouseEvent>(handle, "MouseMovedEvent")
			.def(py::init<const Point&, const Point&>(), "screen_pos"_a, "game_pos"_a);


		py::class_<MouseScrolledEvent, MouseEvent>(handle, "MouseScrolledEvent")
			.def(py::init<const Point&, const Point&, bool>(), "screen_pos"_a, "game_pos"_a, "is_up"_a)
			.def_readonly("is_up", &MouseScrolledEvent::isUp);
	}


	void Export_WindowEvents(py::module_& handle)
	{
		py::class_<WindowClosedEvent, Event>(handle, "WindowClosedEvent")
			.def(py::init<>());

		py::class_<WindowResizedEvent, Event>(handle, "WindowResizedEvent")
			.def(py::init<uint32, uint32>(), "new_width"_a, "new_height"_a)
			.def_readonly("new_width", &WindowResizedEvent::newWidth)
			.def_readonly("new_height", &WindowResizedEvent::newHeight);
	}


	void Export_EventListener(py::module_& handle)
	{
		py::class_<EventListener>(handle, "EventListener")
			.def_property_readonly("id", &EventListener::GetID)

			.def("subscribe_window_closed", &EventListener::Subscribe<WindowClosedEvent>, "func"_a)
			.def("subscribe_window_resized", &EventListener::Subscribe<WindowResizedEvent>, "func"_a)
			.def("subscribe_key_pressed", &EventListener::Subscribe<KeyPressedEvent>, "func"_a)
			.def("subscribe_key_hold", &EventListener::Subscribe<KeyHoldEvent>, "func"_a)
			.def("subscribe_key_released", &EventListener::Subscribe<KeyReleasedEvent>, "func"_a)
			.def("subscribe_mouse_pressed", &EventListener::Subscribe<MousePressedEvent>, "func"_a)
			.def("subscribe_mouse_released", &EventListener::Subscribe<MouseReleasedEvent>, "func"_a)
			.def("subscribe_mouse_moved", &EventListener::Subscribe<MouseMovedEvent>, "func"_a)
			.def("subscribe_mouse_scrolled", &EventListener::Subscribe<MouseScrolledEvent>, "func"_a)

			.def("subscribe_window_closed", &EventListener::UnSubscribe<WindowClosedEvent>)
			.def("unsubscribe_window_resized", &EventListener::UnSubscribe<WindowResizedEvent>)
			.def("unsubscribe_key_pressed", &EventListener::UnSubscribe<KeyPressedEvent>)
			.def("unsubscribe_key_hold", &EventListener::UnSubscribe<KeyHoldEvent>)
			.def("unsubscribe_key_released", &EventListener::UnSubscribe<KeyReleasedEvent>)
			.def("unsubscribe_mouse_pressed", &EventListener::UnSubscribe<MousePressedEvent>)
			.def("unsubscribe_mouse_released", &EventListener::UnSubscribe<MouseReleasedEvent>)
			.def("unsubscribe_mouse_moved", &EventListener::UnSubscribe<MouseMovedEvent>)
			.def("unsubscribe_mouse_scrolled", &EventListener::UnSubscribe<MouseScrolledEvent>)

			.def("unsubscribe_all", &EventListener::UnSubscribeAll);
	}


	void Export_EventHandler(py::module_& handle)
	{
		py::module_ m = handle.def_submodule("EventHandler");
		m.doc() = "Event handler module for Sharpheus";

		m.def("handle", &EventHandler::Handle, "event"_a);
		
		m.def("subscribe_window_closed", &EventHandler::Subscribe<WindowClosedEvent>, "listener_id"_a, "func"_a);
		m.def("subscribe_window_resized", &EventHandler::Subscribe<WindowResizedEvent>, "listener_id"_a, "func"_a);
		m.def("subscribe_key_pressed", &EventHandler::Subscribe<KeyPressedEvent>, "listener_id"_a, "func"_a);
		m.def("subscribe_key_hold", &EventHandler::Subscribe<KeyHoldEvent>, "listener_id"_a, "func"_a);
		m.def("subscribe_key_released", &EventHandler::Subscribe<KeyReleasedEvent>, "listener_id"_a, "func"_a);
		m.def("subscribe_mouse_pressed", &EventHandler::Subscribe<MousePressedEvent>, "listener_id"_a, "func"_a);
		m.def("subscribe_mouse_released", &EventHandler::Subscribe<MouseReleasedEvent>, "listener_id"_a, "func"_a);
		m.def("subscribe_mouse_moved", &EventHandler::Subscribe<MouseMovedEvent>, "listener_id"_a, "func"_a);
		m.def("subscribe_mouse_scrolled", &EventHandler::Subscribe<MouseScrolledEvent>, "listener_id"_a, "func"_a);

		m.def("subscribe_window_closed", &EventHandler::UnSubscribe<WindowClosedEvent>, "listener_id"_a);
		m.def("unsubscribe_window_resized", &EventHandler::UnSubscribe<WindowResizedEvent>, "listener_id"_a);
		m.def("unsubscribe_key_pressed", &EventHandler::UnSubscribe<KeyPressedEvent>, "listener_id"_a);
		m.def("unsubscribe_key_hold", &EventHandler::UnSubscribe<KeyHoldEvent>, "listener_id"_a);
		m.def("unsubscribe_key_released", &EventHandler::UnSubscribe<KeyReleasedEvent>, "listener_id"_a);
		m.def("unsubscribe_mouse_pressed", &EventHandler::UnSubscribe<MousePressedEvent>, "listener_id"_a);
		m.def("unsubscribe_mouse_released", &EventHandler::UnSubscribe<MouseReleasedEvent>, "listener_id"_a);
		m.def("unsubscribe_mouse_moved", &EventHandler::UnSubscribe<MouseMovedEvent>, "listener_id"_a);
		m.def("unsubscribe_mouse_scrolled", &EventHandler::UnSubscribe<MouseScrolledEvent>, "listener_id"_a);

		m.def("unsubscribe_all", &EventHandler::UnSubscribeAll, "listener_id"_a);
	}


	void Export_Events(py::module_& handle)
	{
		Export_Event(handle);
		Export_KeyEvents(handle);
		Export_MouseEvents(handle);
		Export_WindowEvents(handle);
		Export_EventListener(handle);
		Export_EventHandler(handle);
	}

}
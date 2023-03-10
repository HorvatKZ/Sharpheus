#pragma once

#include "Event.hpp"


namespace Sharpheus {

	enum class KeyCode {
		UNKNOWN = -1,

		// Arrows
		RIGHT = 262, LEFT = 263, DOWN = 264, UP = 265,

		// Letters
		A = 65, B = 66, C = 67, D = 68, E = 69, F = 70, G = 71, H = 72, I = 73, J = 74, K = 75, L = 76, M = 77,
		N = 78, O = 79, P = 80, Q = 81, R = 82, S = 83, T = 84, U = 85, V = 86, W = 87, X = 88, Y = 89, Z = 90,

		// Symbols
		SPACE = 32, APOSTROPHE = 39, COMMA = 44, MINUS = 45, PERIOD = 46, SLASH = 47, SEMICOLON = 59, EQUAL = 61,
		LEFT_BRACKET = 91, BACKSLASH = 92, RIGHT_BRACKET = 93, BACKTICK = 96,

		// Numbers
		KEY_0 = 48, KEY_1 = 49, KEY_2 = 50, KEY_3 = 51, KEY_4 = 52, KEY_5 = 53, KEY_6 = 54, KEY_7 = 55, KEY_8 = 56, KEY_9 = 57,

		// Numpad
		NP_0 = 320, NP_1 = 321, NP_2 = 322, NP_3 = 323, NP_4 = 324, NP_5 = 325, NP_6 = 326, NP_7 = 327, NP_8 = 328, NP_9 = 329,
		NP_DECIMAL = 330, NP_DIVIDE = 331, NP_MULTIPLY = 332, NP_SUBTRACT = 333, NP_ADD = 334, NP_ENTER = 335, NP_EQUAL = 336,

		// Controls
		ESCAPE = 256, ENTER = 257, TAB = 258, BACKSPACE = 259, INSERT = 260, DEL = 261, PAGE_UP = 266, PAGE_DOWN = 267, HOME = 268,
		END = 269, CAPS_LOCK = 280, SCROLL_LOCK = 281, NUM_LOCK = 282, PRINT_SCREEN = 283, PAUSE = 284, LEFT_SHIFT = 340, LEFT_CONTROL = 341,
		LEFT_ALT = 342, LEFT_SUPER = 343, RIGHT_SHIFT = 344, RIGHT_CONTROL = 345, RIGHT_ALT = 346, RIGHT_SUPER = 347, MENU = 348,

		// F keys
		F1 = 290, F2 = 291, F3 = 292, F4 = 293, F5 = 294, F6 = 295, F7 = 296, F8 = 297, F9 = 298, F10 = 299,
		F11 = 300, F12 = 301,F13 = 302, F14 = 303, F15 = 304, F16 = 305, F17 = 306, F18 = 307, F19 = 308, F20 = 309,
		F21 = 310, F22 = 311, F23 = 312, F24 = 313, F25 = 314,

		// Non-US keys
		WORLD_1 = 161, WORLD_2 = 162
	
	};

	SPH_DEF_EVENT_FUNC(KeyPressedEvent)
	SPH_DEF_EVENT_FUNC(KeyReleasedEvent)
	SPH_DEF_EVENT_FUNC(KeyHoldEvent)

	
	class SPH_EXPORT KeyEvent : public Event
	{
	public:
		KeyEvent(KeyCode code) : code(code) {}
		KeyEvent(int code) : code((KeyCode)code) {}
		virtual ~KeyEvent() = default;

		const KeyCode code;
	};


	class SPH_EXPORT KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(KeyCode code) : KeyEvent(code) {}
		KeyPressedEvent(int code) : KeyEvent(code) {}
		virtual ~KeyPressedEvent() = default;

		inline std::string ToStr() const override { return "KeyPressedEvent " + std::to_string((int)code); }

		SPH_DECL_EVENT(KeyPressed)
	};


	class SPH_EXPORT KeyHoldEvent : public KeyEvent
	{
	public:
		KeyHoldEvent(KeyCode code) : KeyEvent(code) {}
		KeyHoldEvent(int code) : KeyEvent(code) {}
		virtual ~KeyHoldEvent() = default;

		inline std::string ToStr() const override { return "KeyHoldEvent " + std::to_string((int)code); }

		SPH_DECL_EVENT(KeyHold)
	};


	class SPH_EXPORT KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(KeyCode code) : KeyEvent(code) {}
		KeyReleasedEvent(int code) : KeyEvent(code) {}
		virtual ~KeyReleasedEvent() = default;

		inline std::string ToStr() const override { return "KeyReleasedEvent " + std::to_string((int)code); }

		SPH_DECL_EVENT(KeyReleased)
	};

}
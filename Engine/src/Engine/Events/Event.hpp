#pragma once

#include "pch.h"

#define SPH_DECL_EVENT(type) \
	static Type GetStaticType() { return Type::type; } \
	Type GetType() const override { return Type::type; }

#define SPH_DEF_EVENT_FUNC(eventClass) \
	typedef std::function<void(const class eventClass&)> eventClass##Func;


namespace Sharpheus {

	SPH_DEF_EVENT_FUNC(Event)


	class SPH_EXPORT Event
	{
	public:
		enum class Type {
			None,
			WindowClosed, WindowResized,
			KeyPressed, KeyReleased
		};

		virtual ~Event() = default;

		static Type GetStaticType() { return Type::None; }
		virtual Type GetType() const = 0;
		virtual inline std::string ToStr() const = 0;
	};

}
#pragma once

#include "Engine/Events/LocalEvent.hpp"
#include "Engine/GameObjects/Colliders/Collider.hpp"


namespace Sharpheus {

	SPH_DEF_EVENT_FUNC(CollisionEvent)
	SPH_DEF_EVENT_FUNC(OnEnterEvent)
	SPH_DEF_EVENT_FUNC(OnExitEvent)

	class SPH_EXPORT CollisionEvent : public LocalEvent<Collider>
	{
	public:
		CollisionEvent(Collider* source, Collider* with, const CollData& cd)
			: LocalEvent<Collider>(source), with(with), cd(cd) {}
		virtual ~CollisionEvent() = default;

		inline std::string ToStr() const override {
			return "CollisionEvent from" + source->GetName() + " to " + with->GetName();
		}

		Collider* with;
		CollData cd;
	};


	class SPH_EXPORT OnEnterEvent : public LocalEvent<Collider>
	{
	public:
		OnEnterEvent(Collider* source, GameObject* obj) : LocalEvent<Collider>(source), obj(obj) {}
		virtual ~OnEnterEvent() = default;

		inline std::string ToStr() const override {
			return "OnEnterEvent from " + source->GetName() + ", obj: " + obj->GetName();
		}

		GameObject* obj;
	};


	class SPH_EXPORT OnExitEvent : public LocalEvent<Collider>
	{
	public:
		OnExitEvent(Collider* source, GameObject* obj, bool objDestroyed = false)
			: LocalEvent<Collider>(source), obj(obj), objDestroyed(objDestroyed) {}
		virtual ~OnExitEvent() = default;

		inline std::string ToStr() const override {
			return "OnEnterEvent from " + source->GetName() + ", obj: " + obj->GetName();
		}

		GameObject* obj;
		bool objDestroyed;
	};

}
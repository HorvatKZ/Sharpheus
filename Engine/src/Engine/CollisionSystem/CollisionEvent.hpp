#pragma once

#include "Engine/Events/LocalEvent.hpp"
#include "Engine/GameObjects/Colliders/Collider.hpp"


namespace Sharpheus {

	SPH_DEF_EVENT_FUNC(CollisionEvent)

	class SPH_EXPORT CollisionEvent : public LocalEvent<Collider>
	{
	public:
		CollisionEvent(Collider* source, Collider* with, const Point& direction) : LocalEvent<Collider>(source), with(with), direction(direction) {}
		virtual ~CollisionEvent() = default;

		inline std::string ToStr() const override {
			return "CollisionEvent from" + source->GetName() + " to " + with->GetName() + ", direction: ("
				+ std::to_string(direction.x) + ", " + std::to_string(direction.y) + ")";
		}

		Collider* with;
		Point direction;
	};

}
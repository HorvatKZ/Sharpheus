#pragma once

#include "Engine/Events/LocalEvent.hpp"
#include "Engine/GameObjects/Colliders/Collider.hpp"


namespace Sharpheus {

	SPH_DEF_EVENT_FUNC(CollisionEvent)

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

}
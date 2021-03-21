#pragma once

#include "../ShapedGameObject.hpp"
#include "Engine/CollisionSystem/CollData.hpp"


namespace Sharpheus {

	class CollisionEvent;
	class LocalSourceDestroyedEvent;
	SPH_DEF_EVENT_FUNC(CollisionEvent);
	SPH_DEF_EVENT_FUNC(LocalSourceDestroyedEvent);


	class SPH_EXPORT Collider : public ShapedGameObject
	{
	public:
		Collider(GameObject* parent, const std::string& name, Shape* shape);
		virtual ~Collider();

		virtual CollDataPair CalcCollision(Collider* other);
		virtual void OnCollision(const CollData& cd, Collider* other);
		inline void UpdatePrevPos() { prevPos = worldTrafo.pos; }

		void SetLevel(class Level* level) override;

		inline Point GetVelocity() {
			if (lastDeltaTime == 0) {
				return Point();
			}

			Point diff = worldTrafo.pos - prevPos;
			return Point(diff.x / lastDeltaTime, diff.y / lastDeltaTime);
		}

		inline bool IsDynamic() { return parent->GetType() == Type::PhysicsObject; }
		inline bool WasStill() { return prevPos == worldTrafo.pos; }
		inline bool IsVisible() { return visible; }
		inline void SetVisible(bool visible) { this->visible = visible; }
		inline void Show() { visible = true; }
		inline void Hide() { visible = false; }

		inline void SubscribeCollision(ID subscriberID, CollisionEventFunc&& func1, LocalSourceDestroyedEventFunc&& func2) {
			subscribers[subscriberID] = { func1, func2 };
		}

		inline void UnSubscribeCollision(ID subscriberID) {
			auto it = subscribers.find(subscriberID);
			if (it != subscribers.end()) subscribers.erase(it);
		}

		inline bool operator==(const Collider& other) { return listenerID == other.listenerID; }
		inline bool operator!=(const Collider& other) { return listenerID != other.listenerID; }

	protected:
		Point prevPos;
		bool visible = false;
		float furthest = 0.f;
		float lastDeltaTime = 0.f;
		std::unordered_map<ID, std::pair<CollisionEventFunc, LocalSourceDestroyedEventFunc>> subscribers;

		static Color shapeColor;

		virtual void Tick(float deltaTime) override;
		virtual void Render() override;
		virtual void RenderShape() = 0;
	};

}
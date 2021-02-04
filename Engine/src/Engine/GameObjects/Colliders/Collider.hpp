#pragma once

#include "../GameObject.hpp"


namespace Sharpheus {

	class CollisionEvent;
	class LocalSourceDestroyedEvent;
	SPH_DEF_EVENT_FUNC(CollisionEvent);
	SPH_DEF_EVENT_FUNC(LocalSourceDestroyedEvent);


	class SPH_EXPORT Collider : public GameObject
	{
	public:
		Collider(GameObject* parent, const std::string& name, const Transform& trafo);
		virtual ~Collider();

		virtual std::pair<Point, Point> CalcCollision(Collider* other) = 0;
		virtual void OnCollision(const CollisionEvent& e) = 0;
		void UpdatePrevPos();

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
		bool visible;
		float radius;
		std::unordered_map<ID, std::pair<CollisionEventFunc, LocalSourceDestroyedEventFunc>> subscribers;

		static Color shapeColor;

		virtual void TickThis(float deltaTime) override;
		virtual void RenderThis() override;
		virtual void RenderShape() = 0;
	};

}
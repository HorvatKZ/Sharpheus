#pragma once

#include "../ShapedGameObject.hpp"
#include "Engine/CollisionSystem/CollData.hpp"


namespace Sharpheus {

	class CollisionEvent;
	class OnEnterEvent;
	class OnExitEvent;
	SPH_DEF_EVENT_FUNC(CollisionEvent);
	SPH_DEF_EVENT_FUNC(OnEnterEvent);
	SPH_DEF_EVENT_FUNC(OnExitEvent);


	class SPH_EXPORT Collider : public ShapedGameObject
	{
	public:
		Collider(GameObject* parent, const std::string& name, Shape* shape);
		virtual ~Collider();
		virtual void CopyFrom(GameObject* other) override;

		virtual CollDataPair CalcCollision(Collider* other);
		virtual void OnCollision(const CollData& cd, Collider* other);

		virtual void CheckTriggering(GameObject* obj);

		void SetLevel(class Level* level) override;

		inline Point GetVelocity() {
			if (lastDeltaTime == 0) {
				return Point();
			}

			Point diff = worldTrafo.pos - lastPos;
			return Point(diff.x / lastDeltaTime, diff.y / lastDeltaTime);
		}

		inline bool IsTrigger() { return isTrigger; }
		inline void SetTrigger(bool isTrigger) { this->isTrigger = isTrigger; }

		inline bool IsDynamic() { return parent->Is(Type::PhysicsObject); }
		inline bool IsColliderVisible() { return colliderVisible; }
		inline void SetColliderVisible(bool visible) { this->colliderVisible = visible; }
		inline void Show() { colliderVisible = true; }
		inline void Hide() { colliderVisible = false; }

		inline void SubscribeCollision(ID subscriberID, CollisionEventFunc&& func) {
			onCollisionSubscribers[subscriberID] = std::move(func);
		}
		inline void UnSubscribeCollision(ID subscriberID) {
			auto it = onCollisionSubscribers.find(subscriberID);
			if (it != onCollisionSubscribers.end()) onCollisionSubscribers.erase(it);
		}
		inline void SubscribeTriggerEnter(ID subscriberID, OnEnterEventFunc&& func) {
			onTriggerEnterSubscribers[subscriberID] = std::move(func);
		}
		inline void UnSubscribeTriggerEnter(ID subscriberID) {
			auto it = onTriggerEnterSubscribers.find(subscriberID);
			if (it != onTriggerEnterSubscribers.end()) onTriggerEnterSubscribers.erase(it);
		}
		inline void SubscribeTriggerExit(ID subscriberID, OnExitEventFunc&& func) {
			onTriggerExitSubscribers[subscriberID] = std::move(func);
		}
		inline void UnSubscribeTriggerExit(ID subscriberID) {
			auto it = onTriggerExitSubscribers.find(subscriberID);
			if (it != onTriggerExitSubscribers.end()) onTriggerExitSubscribers.erase(it);
		}

		inline void SwapMaps() {
			auto tmp = lastInside;
			lastInside = currInside;
			currInside = tmp;
			currInside->clear();
		}

		inline void RenderShapeIfColliderVisible() {
			if (IsAllVisible()) {
				RenderShape();
			}
		}

		virtual bool Load(FileLoader& fl) override;

	protected:
		bool colliderVisible = false;
		bool isTrigger = false;
		float furthest = 0.f;
		float lastDeltaTime = 0.f;
		std::unordered_map<ID, CollisionEventFunc> onCollisionSubscribers;
		std::unordered_map<ID, OnEnterEventFunc> onTriggerEnterSubscribers;
		std::unordered_map<ID, OnExitEventFunc> onTriggerExitSubscribers;

		std::unordered_map<ID, GameObject*> insideTriggerA, insideTriggerB, *currInside, *lastInside;

		static const Color colliderColor;
		static const Color triggerColor;

		virtual bool Save(FileSaver& fs) override;

		virtual void Tick(float deltaTime) override;
		virtual void RenderShape() = 0;

		virtual void OnObjectDestroyed(const GameObjectDestroyedEvent& e);
		virtual void OnTriggerEnter(GameObject* obj);
		virtual void OnTriggerExit(GameObject* obj, bool objDestroyed = false);
	};

}
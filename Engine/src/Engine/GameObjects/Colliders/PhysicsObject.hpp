#pragma once

#include "../GameObject.hpp"
#include "Engine/CollisionSystem/CollData.hpp"


namespace Sharpheus {

	class SPH_EXPORT PhysicsObject : public GameObject
	{
		friend class Collider;

	public:
		PhysicsObject(GameObject* parent, const std::string& name);
		virtual ~PhysicsObject() = default;
		virtual void CopyFrom(GameObject * other) override;

		inline Point& GetVelocity() { return velocity; }
		inline float GetMass() { return mass; }
		inline float GetGravity() { return gravity; }
		inline float GetBounce() { return bounce; }
		inline float GetFriction() { return friction; }

		inline void AddVelocity(const Point& velocity) { this->velocity += velocity; }
		inline void SetVelocity(const Point& velocity) { this->velocity = velocity; }
		inline void SetVelocityX(float x) { velocity.x = x; }
		inline void SetVelocityY(float y) { velocity.y = y; }
		inline void SetGravity(float gravity) { this->gravity = gravity; }
		inline void SetMass(float mass) {
			if (mass > 0.f) {
				this->mass = mass;
			}
		}
		inline void SetBounce(float bounce) {
			this->bounce = glm::clamp(bounce, 0.f, 1.f);
		}
		inline void SetFriction(float friction) {
			this->friction = glm::clamp(friction, 0.f, 1.f);
		}

		virtual bool Load(FileLoader& fl) override;

		SPH_DECL_GAMEOBJECT(PhysicsObject)

	protected:
		Point velocity = Point(0, 0);
		float mass = 1.f;
		float gravity = 0.f;
		float bounce = 1.f;
		float friction = 0.f;

		virtual bool Save(FileSaver& fs) override;

		virtual void Tick(float deltaTime) override;

		void ResolveCollision(const CollData& cd);
	};

}
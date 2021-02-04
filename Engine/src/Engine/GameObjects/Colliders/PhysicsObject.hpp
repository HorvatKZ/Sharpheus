#pragma once

#include "../GameObject.hpp"

namespace Sharpheus {

	class PhysicsObject : public GameObject
	{
		friend class Collider;

	public:
		PhysicsObject(GameObject* parent, const std::string& name, const Transform& trafo);
		virtual ~PhysicsObject() = default;

		inline Point& GetVelocity() { return velocity; }
		inline float GetMass() { return mass; }
		inline float GetGravity() { return gravity; }

		inline void AddVelocity(const Point& velocity) { this->velocity += velocity; }
		inline void SetVelocity(const Point& velocity) { this->velocity = velocity; }
		inline void SetVelocityX(float x) { velocity.x = x; }
		inline void SetVelocityY(float y) { velocity.y = y; }
		inline void Reflect(float angle, bool clockwise) {
			velocity = velocity.Rotate((clockwise ? 2 : -2) * angle) * abs(glm::cos(glm::radians(angle)));
		}

		inline void SetMass(float mass) { this->mass = mass; }
		inline void SetGravity(float gravity) { this->gravity = gravity; }

		SPH_DECL_GAMEOBJECT(PhysicsObject)

	protected:
		Point velocity = Point(0, 0);
		float mass = 1.f;
		float gravity = 1.f;

		virtual void TickThis(float deltaTime) override;
		virtual void RenderThis() override;
	};

}
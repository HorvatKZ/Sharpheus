#include "pch.h"
#include "PhysicsObject.hpp"


namespace Sharpheus {

	ClassInfo PhysicsObject::classInfo("PhysicsObject", "physicsobj.png", {
		new PointProvider<PhysicsObject>("Velocity", SPH_BIND_GETTER(PhysicsObject::GetVelocity), SPH_BIND_SETTER(PhysicsObject::SetVelocity)),
		new UFloatProvider<PhysicsObject>("Mass", SPH_BIND_GETTER(PhysicsObject::GetMass), SPH_BIND_SETTER(PhysicsObject::SetMass)),
		new FloatProvider<PhysicsObject>("Gravity", SPH_BIND_GETTER(PhysicsObject::GetGravity), SPH_BIND_SETTER(PhysicsObject::SetGravity))
	});


	PhysicsObject::PhysicsObject(GameObject* parent, const std::string& name) :
		GameObject(parent, name) {}

	void PhysicsObject::Tick(float deltaTime) {
		trafo.pos += velocity * deltaTime;
		SetTrafo(trafo);
		velocity += gravity * deltaTime * Point::Down;
	}

	void PhysicsObject::Render() {}
}
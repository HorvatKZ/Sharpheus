#include "pch.h"
#include "PhysicsObject.hpp"


namespace Sharpheus {

	PhysicsObject::PhysicsObject(GameObject* parent, const std::string& name, const Transform& trafo) :
		GameObject(parent, name, trafo) {}

	void PhysicsObject::TickThis(float deltaTime) {
		trafo.pos += velocity * deltaTime;
		SetTrafo(trafo);
		velocity += gravity * deltaTime * Point::Down;
	}

	void PhysicsObject::RenderThis() {}
}
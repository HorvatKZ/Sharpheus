#include "pch.h"
#include "PhysicsObject.hpp"


namespace Sharpheus {

	ClassInfo PhysicsObject::classInfo("PhysicsObject", "physicsobj.png", {
		new PointProvider<PhysicsObject>("Velocity", SPH_BIND_GETTER(PhysicsObject::GetVelocity), SPH_BIND_SETTER(PhysicsObject::SetVelocity)),
		new UFloatProvider<PhysicsObject>("Mass", SPH_BIND_GETTER(PhysicsObject::GetMass), SPH_BIND_SETTER(PhysicsObject::SetMass)),
		new FloatProvider<PhysicsObject>("Gravity", SPH_BIND_GETTER(PhysicsObject::GetGravity), SPH_BIND_SETTER(PhysicsObject::SetGravity)),
		new UFloatProvider<PhysicsObject>("Bounce", SPH_BIND_GETTER(PhysicsObject::GetBounce), SPH_BIND_SETTER(PhysicsObject::SetBounce)),
		new UFloatProvider<PhysicsObject>("Friction", SPH_BIND_GETTER(PhysicsObject::GetFriction), SPH_BIND_SETTER(PhysicsObject::SetFriction))
	});


	PhysicsObject::PhysicsObject(GameObject* parent, const std::string& name) :
		GameObject(parent, name) {}


	void PhysicsObject::CopyFrom(GameObject* other)
	{
		SPH_CHECKTYPE(other, PhysicsObject);

		GameObject::CopyFrom(other);
		PhysicsObject* trueOther = (PhysicsObject*)other;
		velocity = trueOther->velocity;
		gravity = trueOther->gravity;
		mass = trueOther->mass;
		bounce = trueOther->bounce;
		friction = trueOther->friction;
	}


	void PhysicsObject::ResolveCollision(const CollData& cd)
	{
		// Correction
		Transform newTrafo = worldTrafo;
		newTrafo.pos -= cd.geom.depth * cd.geom.normal;
		SetWorldTrafo(newTrafo);

		// Impulse
		Point vR = cd.vOther - velocity;
		float imp = -(1.f + bounce) * (vR * cd.geom.normal);
		if (cd.mOther > 0.f) {
			velocity -= (imp * cd.mOther / (mass + cd.mOther)) * cd.geom.normal;
		}
		else {
			velocity -= imp * cd.geom.normal;
		}
	}


	bool PhysicsObject::Save(FileSaver& fs)
	{
		GameObject::Save(fs);
		fs.Write(velocity);
		fs.Write(mass);
		fs.Write(gravity);
		fs.Write(bounce);
		fs.Write(friction);
		return fs.GetStatus();
	}


	bool PhysicsObject::Load(FileLoader& fl)
	{
		GameObject::Load(fl);
		fl.Read(velocity);
		fl.Read(mass);
		fl.Read(gravity);
		fl.Read(bounce);
		fl.Read(friction);
		return fl.GetStatus();
	}


	void PhysicsObject::Tick(float deltaTime) {
		Transform newTrafo = trafo;
		newTrafo.pos += velocity * deltaTime;
		SetTrafo(newTrafo);
		velocity += gravity * deltaTime * Point::Down;
		velocity *= 1.f - friction * deltaTime;
	}

}
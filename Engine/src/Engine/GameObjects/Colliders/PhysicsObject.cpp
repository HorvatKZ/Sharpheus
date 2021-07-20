#include "pch.h"
#include "PhysicsObject.hpp"


namespace Sharpheus {

	SPH_START_CLASSINFO(PhysicsObject, "physicsobj.png")
		SPH_PROVIDE_POINT(PhysicsObject, "Velocity", GetVelocity, SetVelocity)
		SPH_PROVIDE_UFLOAT(PhysicsObject, "Mass", GetMass, SetMass)
		SPH_PROVIDE_FLOAT(PhysicsObject, "Gravity", GetGravity, SetGravity)
		SPH_PROVIDE_UFLOAT_RANGE(PhysicsObject, "Bounce", GetBounce, SetBounce, 0.f, 1.f)
		SPH_PROVIDE_UFLOAT_RANGE(PhysicsObject, "Friction", GetFriction, SetFriction, 0.f, 1.f)
	SPH_END_CLASSINFO


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
		} else {
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
		Transform newTrafo = worldTrafo;
		newTrafo.pos += velocity * deltaTime;
		SetWorldTrafo(newTrafo);
		velocity += gravity * deltaTime * Point::Down;
		velocity *= 1.f - friction * deltaTime;
	}

}
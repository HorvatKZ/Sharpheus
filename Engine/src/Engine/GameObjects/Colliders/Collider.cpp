#include "pch.h"
#include "Collider.hpp"
#include "Engine/CollisionSystem/CollisionSystem.hpp"
#include "Engine/Level.hpp"


namespace Sharpheus {

	Color Collider::shapeColor = Color(0, 255, 64, 127);


	Collider::Collider(GameObject* parent, const std::string& name, Shape* shape) : ShapedGameObject(parent, name, shape)
	{
		prevPos = worldTrafo.pos;
	}


	Collider::~Collider()
	{
		level->GetCollSys().RemoveCollider(this);

		LocalSourceDestroyedEvent e(GetID());
		for (auto it = subscribers.begin(); it != subscribers.end(); ++it) {
			(*it).second.second(e);
		}
	}


	CollDataPair Collider::CalcCollision(Collider* other)
	{
		if (shape->IsTooFarFrom(other->shape)) {
			return { CollData(), CollData() };
		}

		CollData thisColl;
		thisColl.geom = shape->GetIntersectionWith(other->shape);
		CollData otherColl = thisColl;
		thisColl.vOther = other->GetVelocity();
		otherColl.vOther = GetVelocity();
		otherColl.geom.normal *= -1;

		if (IsDynamic()) {
			thisColl.isDynamic = true;
			otherColl.mOther = ((PhysicsObject*)parent)->mass;
		}
		if (other->IsDynamic()) {
			otherColl.isDynamic = true;
			thisColl.mOther = ((PhysicsObject*)other->parent)->mass;
		}

		if (thisColl.isDynamic && !otherColl.isDynamic) {
			thisColl.geom.depth *= 2;
			otherColl.geom.depth = 0.f;
		}
		if (!thisColl.isDynamic && otherColl.isDynamic) {
			otherColl.geom.depth *= 2;
			thisColl.geom.depth = 0.f;
		}

		return { thisColl, otherColl };
	}


	void Collider::OnCollision(const CollData& cd, Collider* other)
	{
		if (IsDynamic()) {
			((PhysicsObject*)parent)->ResolveCollision(cd);
		}

		CollisionEvent e(this, other, cd);
		for (auto it = subscribers.begin(); it != subscribers.end(); ++it) {
			(*it).second.first(e);
		}
	}


	void Collider::SetLevel(Level* level)
	{
		GameObject::SetLevel(level);
		level->GetCollSys().AddCollider(this);
	}


	void Collider::Tick(float deltaTime)
	{
		lastDeltaTime = deltaTime;
	}


	void Collider::Render()
	{
		if (visible) {
			RenderShape();
		}
	}
}
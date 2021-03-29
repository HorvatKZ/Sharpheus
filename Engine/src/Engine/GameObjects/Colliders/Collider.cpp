#include "pch.h"
#include "Collider.hpp"
#include "Engine/CollisionSystem/CollisionSystem.hpp"
#include "Engine/Level.hpp"


namespace Sharpheus {

	Color Collider::colliderColor = Color(0, 255, 64, 127);
	Color Collider::triggerColor = Color(167, 15, 255, 127);


	Collider::Collider(GameObject* parent, const std::string& name, Shape* shape) : ShapedGameObject(parent, name, shape)
	{
		currInside = &insideTriggerA;
		lastInside = &insideTriggerB;
	}


	Collider::~Collider()
	{
		level->GetCollSys().RemoveCollider(this);

		for (auto it = currInside->begin(); it != currInside->end(); ++it) {
			(*it).second->UnSubscribeForDestruction(GetID());
		}
		for (auto it = lastInside->begin(); it != lastInside->end(); ++it) {
			(*it).second->UnSubscribeForDestruction(GetID());
		}
	}


	void Collider::CopyFrom(GameObject* other)
	{
		SPH_CHECKMASK(other, Collider);

		ShapedGameObject::CopyFrom(other);
		Collider* trueOther = (Collider*)other;
		isTrigger = trueOther->isTrigger;
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
		for (auto it = onCollisionSubscribers.begin(); it != onCollisionSubscribers.end(); ++it) {
			(*it).second(e);
		}
	}


	void Collider::CheckTriggering(GameObject* obj)
	{
		if (obj != this && shape->IsInside(obj->GetWorldTrafo().pos)) {
			currInside->insert({ obj->GetID(), obj });
			if (lastInside->find(obj->GetID()) == lastInside->end()) {
				obj->SubscribeForDestruction(GetID(), SPH_BIND(Collider::OnObjectDestroyed));
				OnTriggerEnter(obj);
			}
		}
		else if (lastInside->find(obj->GetID()) != lastInside->end()) {
			obj->UnSubscribeForDestruction(GetID());
			OnTriggerExit(obj);
		}

		for (GameObject* child : obj->GetChildren()) {
			CheckTriggering(child);
		}
	}


	void Collider::SetLevel(Level* level)
	{
		GameObject::SetLevel(level);
		level->GetCollSys().AddCollider(this);
	}


	bool Collider::Load(FileLoader& fl)
	{
		ShapedGameObject::Load(fl);
		fl.Read(isTrigger);
		return fl.GetStatus();
	}


	bool Collider::Save(FileSaver& fs)
	{
		ShapedGameObject::Save(fs);
		fs.Write(isTrigger);
		return fs.GetStatus();
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


	void Collider::OnObjectDestroyed(const GameObjectDestroyedEvent& e)
	{
		ID objID = e.source->GetID();
		currInside->erase(objID);
		lastInside->erase(objID);
		OnTriggerExit(e.source, true);
	}


	void Collider::OnTriggerEnter(GameObject* obj)
	{
		OnEnterEvent e(this, obj);
		for (auto it = onTriggerEnterSubscribers.begin(); it != onTriggerEnterSubscribers.end(); ++it) {
			(*it).second(e);
		}
	}


	void Collider::OnTriggerExit(GameObject* obj, bool objDestroyed)
	{
		OnExitEvent e(this, obj, objDestroyed);
		for (auto it = onTriggerExitSubscribers.begin(); it != onTriggerExitSubscribers.end(); ++it) {
			(*it).second(e);
		}
	}

}
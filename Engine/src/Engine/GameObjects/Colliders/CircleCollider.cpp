#include "pch.h"
#include "CircleCollider.hpp"
#include "PhysicsObject.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/CollisionSystem/CollisionEvent.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"


namespace Sharpheus {

	ClassInfo CircleCollider::classInfo("CircleCollider", "circlecollider.png", {
		new BoolProvider<CircleCollider>("Only Trigger", SPH_BIND_GETTER(CircleCollider::IsTrigger), SPH_BIND_SETTER(CircleCollider::SetTrigger)),
		new UFloatProvider<CircleCollider>("Radius", SPH_BIND_GETTER(CircleCollider::GetRadius), SPH_BIND_SETTER(CircleCollider::SetRadius))
	});


	CircleCollider::CircleCollider(GameObject* parent, const std::string& name)
		: Collider(parent, name, new Oval())
	{
	}


	CircleCollider::~CircleCollider()
	{
	}


	void CircleCollider::CopyFrom(GameObject* other)
	{
		SPH_CHECKTYPE(other, CircleCollider);

		Collider::CopyFrom(other);
		CircleCollider* trueOther = (CircleCollider*)other;
		SetRadius(trueOther->radius);
	}


	void CircleCollider::RenderSelection()
	{
		ResourceManager::GetCircle()->Render(shape->GetCorners(), selectColor);
	}


	bool CircleCollider::Save(FileSaver& fs)
	{
		Collider::Save(fs);
		fs.Write(radius);
		return fs.GetStatus();
	}


	bool CircleCollider::Load(FileLoader& fl)
	{
		Collider::Load(fl);
		fl.Read(radius);
		SetRadius(radius);
		return fl.GetStatus();
	}


	void CircleCollider::RenderShape()
	{
		ResourceManager::GetCircle()->Render(shape->GetCorners(), isTrigger ? triggerColor : colliderColor);
	}

}
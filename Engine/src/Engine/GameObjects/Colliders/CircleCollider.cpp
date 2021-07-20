#include "pch.h"
#include "CircleCollider.hpp"
#include "PhysicsObject.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/CollisionSystem/CollisionEvent.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"


namespace Sharpheus {

	SPH_START_CLASSINFO(CircleCollider, "circlecollider.png")
		SPH_PROVIDE_BOOL(CircleCollider, "Only Trigger", IsTrigger, SetTrigger)
		SPH_PROVIDE_UFLOAT(CircleCollider, "Radius", GetRadius, SetRadius)
	SPH_END_CLASSINFO


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
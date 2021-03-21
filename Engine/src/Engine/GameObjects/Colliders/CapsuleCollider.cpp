#include "pch.h"
#include "CapsuleCollider.hpp"
#include "PhysicsObject.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/CollisionSystem/CollisionEvent.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"


namespace Sharpheus {

	ClassInfo CapsuleCollider::classInfo("CapsuleCollider", "capsulecollider.png", {
		new UFloatProvider<CapsuleCollider>("Width", SPH_BIND_GETTER(CapsuleCollider::GetWidth), SPH_BIND_SETTER(CapsuleCollider::SetWidth)),
		new UFloatProvider<CapsuleCollider>("Height", SPH_BIND_GETTER(CapsuleCollider::GetHeight), SPH_BIND_SETTER(CapsuleCollider::SetHeight))
		});


	Point CapsuleCollider::upperTexCoords[] = { Point(0, 0), Point(1, 0), Point(1, 0.5), Point(0, 0.5) };
	Point CapsuleCollider::lowerTexCoords[] = { Point(0, 0.5), Point(1, 0.5), Point(1, 1), Point(0, 1) };

	CapsuleCollider::CapsuleCollider(GameObject* parent, const std::string& name)
		: Collider(parent, name, new Capsule())
	{
	}


	CapsuleCollider::~CapsuleCollider()
	{
	}


	void CapsuleCollider::CopyFrom(GameObject* other)
	{
		SPH_CHECKTYPE(other, CapsuleCollider);

		Collider::CopyFrom(other);
		CapsuleCollider* trueOther = (CapsuleCollider*)other;
		width = trueOther->width;
		height = trueOther->height;
		SetSizer(width, height);
	}


	void CapsuleCollider::SetWorldTrafo(const Transform& trafo)
	{
		Collider::SetWorldTrafo(trafo);
		UpdateCapsuleDomeHeight();
	}


	void CapsuleCollider::RenderSelection()
	{
		RenderCapsule(selectColor);
	}


	bool CapsuleCollider::Save(FileSaver& fs)
	{
		GameObject::Save(fs);
		fs.Write(width);
		fs.Write(height);
		return fs.GetStatus();
	}


	bool CapsuleCollider::Load(FileLoader& fl)
	{
		GameObject::Load(fl);
		fl.Read(width);
		fl.Read(height);
		SetSizer(width, height);
		return fl.GetStatus();
	}

	void CapsuleCollider::RenderShape()
	{
		RenderCapsule(shapeColor);
	}


	void CapsuleCollider::RenderCapsule(const Color& color)
	{
		Point* corners = shape->GetCorners();
		Point* innerCorners = ((Capsule*)shape)->GetInnerCorners();
		Renderer::DrawMonocromeQuad(innerCorners, color);

		Point upper[] = { corners[0], corners[1], innerCorners[1], innerCorners[0] };
		Point lower[] = { innerCorners[3], innerCorners[2], corners[2], corners[3] };
		ResourceManager::GetCircle()->RenderPart(upper, upperTexCoords, color);
		ResourceManager::GetCircle()->RenderPart(lower, lowerTexCoords, color);
	}


	void CapsuleCollider::UpdateWorldTrafo(const Transform& parentWorldTrafo)
	{
		Collider::UpdateWorldTrafo(parentWorldTrafo);
		UpdateCapsuleDomeHeight();
	}


	void CapsuleCollider::UpdateCapsuleDomeHeight()
	{
		((Capsule*)shape)->SetDomeHeight(this->width * worldTrafo.scale.y / 2);
	}

}
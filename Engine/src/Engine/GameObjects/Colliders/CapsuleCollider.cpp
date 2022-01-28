#include "pch.h"
#include "CapsuleCollider.hpp"
#include "PhysicsObject.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/CollisionSystem/CollisionEvent.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"


namespace Sharpheus {

	SPH_START_CLASSINFO(CapsuleCollider, "capsulecollider.png")
		SPH_PROVIDE_BOOL(CapsuleCollider, "Only Trigger", IsTrigger, SetTrigger)
		SPH_PROVIDE_UFLOAT(CapsuleCollider, "Width", GetWidth, SetWidth)
		SPH_PROVIDE_UFLOAT(CapsuleCollider, "Height", GetHeight, SetHeight)
	SPH_END_CLASSINFO


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
		Collider::Save(fs);
		fs.Write(width);
		fs.Write(height);
		return fs.GetStatus();
	}


	bool CapsuleCollider::Load(FileLoader& fl)
	{
		Collider::Load(fl);
		fl.Read(width);
		fl.Read(height);
		SetSizer(width, height);
		return fl.GetStatus();
	}

	void CapsuleCollider::RenderShape()
	{
		RenderCapsule(isTrigger ? triggerColor : colliderColor);
	}


	void CapsuleCollider::RenderCapsule(const Color& color)
	{
		Point* corners = shape->GetCorners();
		Point* innerCorners = ((Capsule*)shape)->GetInnerCorners();
		Renderer::DrawMonocromeQuad(innerCorners, color);

		Point upper[] = { corners[0], corners[1], innerCorners[1], innerCorners[0] };
		Point lower[] = { innerCorners[3], innerCorners[2], corners[2], corners[3] };
		Renderer::DrawCirclePart(upper, upperTexCoords, color);
		Renderer::DrawCirclePart(lower, lowerTexCoords, color);
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
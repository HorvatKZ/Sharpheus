#include "pch.h"
#include "BoxCollider.hpp"
#include "PhysicsObject.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/CollisionSystem/CollisionEvent.hpp"


namespace Sharpheus {

	SPH_START_CLASSINFO(BoxCollider, "boxcollider.png")
		SPH_PROVIDE_BOOL(BoxCollider, "Only Trigger", IsTrigger, SetTrigger)
		SPH_PROVIDE_UFLOAT(BoxCollider, "Width", GetWidth, SetWidth)
		SPH_PROVIDE_UFLOAT(BoxCollider, "Height", GetHeight, SetHeight)
	SPH_END_CLASSINFO


	BoxCollider::BoxCollider(GameObject* parent, const std::string& name)
		: Collider(parent, name, new Rect())
	{
	}


	BoxCollider::~BoxCollider()
	{
	}


	void BoxCollider::CopyFrom(GameObject* other)
	{
		SPH_CHECKTYPE(other, BoxCollider);

		Collider::CopyFrom(other);
		BoxCollider* trueOther = (BoxCollider*)other;
		width = trueOther->width;
		height = trueOther->height;
		SetSizer(width, height);
	}


	void BoxCollider::RenderSelection()
	{
		ShapedGameObject::RenderSelection();
	}


	bool BoxCollider::Save(FileSaver& fs)
	{
		Collider::Save(fs);
		fs.Write(width);
		fs.Write(height);
		return fs.GetStatus();
	}


	bool BoxCollider::Load(FileLoader& fl)
	{
		Collider::Load(fl);
		fl.Read(width);
		fl.Read(height);
		SetSizer(width, height);
		return fl.GetStatus();
	}


	void BoxCollider::RenderShape()
	{
		Renderer::DrawMonocromeQuad(shape->GetCorners(), isTrigger ? triggerColor : colliderColor);
	}

}
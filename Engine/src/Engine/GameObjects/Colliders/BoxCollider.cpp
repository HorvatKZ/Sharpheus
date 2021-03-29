#include "pch.h"
#include "BoxCollider.hpp"
#include "PhysicsObject.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/CollisionSystem/CollisionEvent.hpp"


namespace Sharpheus {

	ClassInfo BoxCollider::classInfo("BoxCollider", "boxcollider.png", {
		new BoolProvider<BoxCollider>("Only Trigger", SPH_BIND_GETTER(BoxCollider::IsTrigger), SPH_BIND_SETTER(BoxCollider::SetTrigger)),
		new UFloatProvider<BoxCollider>("Width", SPH_BIND_GETTER(BoxCollider::GetWidth), SPH_BIND_SETTER(BoxCollider::SetWidth)),
		new UFloatProvider<BoxCollider>("Height", SPH_BIND_GETTER(BoxCollider::GetHeight), SPH_BIND_SETTER(BoxCollider::SetHeight))
	});


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
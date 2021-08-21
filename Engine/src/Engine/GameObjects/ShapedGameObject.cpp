#include "pch.h"
#include "ShapedGameObject.hpp"
#include "Engine/Renderer/Renderer.hpp"


namespace Sharpheus {

	ShapedGameObject::ShapedGameObject(GameObject* parent, const std::string& name, Shape* shape)
		: GameObject(parent, name), shape(shape)
	{
		UpdateShape();
	}


	ShapedGameObject::~ShapedGameObject()
	{
		delete shape;
	}


	void ShapedGameObject::SetWorldTrafo(const Transform& trafo)
	{
		GameObject::SetWorldTrafo(trafo);
		UpdateShape();
	}


	void ShapedGameObject::RenderSelection()
	{
		Renderer::DrawMonocromeQuad(shape->GetCorners(), selectColor);
	}


	bool ShapedGameObject::IsSelected(const Point& pos)
	{
		if (!IsAllVisible()) {
			return false;
		}

		if (shape == nullptr) {
			return false;
		}

		return shape->IsInside(pos);
	}


	void ShapedGameObject::UpdateWorldTrafo(const Transform& parentWorldTrafo)
	{
		GameObject::UpdateWorldTrafo(parentWorldTrafo);
		UpdateShape();
	}


	void ShapedGameObject::UpdateShape()
	{
		shape->SetPos(worldTrafo.pos);
		shape->SetDim(Point(sizer.x / 2 * worldTrafo.scale.x, sizer.y / 2 * worldTrafo.scale.y));
		shape->SetRot(worldTrafo.rot);

		if (shape->GetType() == Shape::Type::CAPSULE) {
			((Capsule*)shape)->SetDomeHeight(sizer.x / 2 * worldTrafo.scale.y);
		}
	}

}

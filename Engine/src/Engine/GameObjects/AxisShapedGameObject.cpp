#include "pch.h"
#include "AxisShapedGameObject.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"


namespace Sharpheus {

	AxisShapedGameObject::AxisShapedGameObject(GameObject* parent, const std::string& name, Shape* shape) :
		ShapedGameObject(parent, name, shape)
	{
		ForceUpdateAxes();
	}


	void AxisShapedGameObject::CopyFrom(GameObject* other)
	{
		ShapedGameObject::CopyFrom(other);
		UpdateSizer();
	}


	void AxisShapedGameObject::SetWorldTrafo(const Transform& trafo)
	{
		Transform oldTrafo = worldTrafo;
		ShapedGameObject::SetWorldTrafo(trafo);
		UpdateAxes(oldTrafo);
	}


	void AxisShapedGameObject::UpdateWorldTrafo(const Transform& parentWorldTrafo)
	{
		Transform oldTrafo = worldTrafo;
		ShapedGameObject::UpdateWorldTrafo(parentWorldTrafo);
		UpdateAxes(oldTrafo);
	}


	void AxisShapedGameObject::UpdateAxes(const Transform& oldTrafo)
	{
		if (oldTrafo.rot != worldTrafo.rot || oldTrafo.scale != worldTrafo.scale) {
			ForceUpdateAxes();
		}
	}


	void AxisShapedGameObject::ForceUpdateAxes()
	{
		xAxis = Point::GetUnit(worldTrafo.rot) * worldTrafo.scale.x;
		yAxis = Point::GetUnit(worldTrafo.rot - 90) * worldTrafo.scale.y;
	}

}
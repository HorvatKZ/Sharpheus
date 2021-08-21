#include "pch.h"
#include "AxisGameObject.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"


namespace Sharpheus {

	AxisGameObject::AxisGameObject(GameObject* parent, const std::string& name, Shape* shape) :
		RenderableGameObject(parent, name, shape)
	{
		ForceUpdateAxes();
	}


	void AxisGameObject::CopyFrom(GameObject* other)
	{
		RenderableGameObject::CopyFrom(other);
		UpdateSizer();
	}


	void AxisGameObject::SetWorldTrafo(const Transform& trafo)
	{
		Transform oldTrafo = worldTrafo;
		RenderableGameObject::SetWorldTrafo(trafo);
		UpdateAxes(oldTrafo);
		UpdateSizer();
	}


	void AxisGameObject::UpdateWorldTrafo(const Transform& parentWorldTrafo)
	{
		Transform oldTrafo = worldTrafo;
		RenderableGameObject::UpdateWorldTrafo(parentWorldTrafo);
		UpdateAxes(oldTrafo);
		UpdateSizer();
	}


	void AxisGameObject::UpdateAxes(const Transform& oldTrafo)
	{
		if (oldTrafo.rot != worldTrafo.rot || oldTrafo.scale != worldTrafo.scale) {
			ForceUpdateAxes();
		}
	}


	void AxisGameObject::ForceUpdateAxes()
	{
		xAxis = Point::GetUnit(worldTrafo.rot) * worldTrafo.scale.x;
		yAxis = Point::GetUnit(worldTrafo.rot - 90) * worldTrafo.scale.y;
	}

}
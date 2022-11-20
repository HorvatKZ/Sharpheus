#include "pch.h"
#include "Circle.hpp"
#include "Engine/Renderer/Renderer.hpp"


namespace Sharpheus {

	SPH_START_CLASSINFO(Circle, "circle.png")
		SPH_PROVIDE_LAYER(Circle, "Layer", GetLayer, SetLayer)
		SPH_PROVIDE_UFLOAT(Circle, "Radius", GetRadius, SetRadius)
		SPH_PROVIDE_COLOR(Circle, "Color", GetColor, SetColor)
	SPH_END_CLASSINFO


	Circle::Circle(GameObject* parent, const std::string& name) :
		RenderableGameObject(parent, name, new Oval()) {}


	void Circle::CopyFrom(GameObject* other)
	{
		SPH_CHECKTYPE(other, Circle);

		RenderableGameObject::CopyFrom(other);
		Circle* trueOther = (Circle*)other;
		radius = trueOther->radius;
		color = trueOther->color;
		SetSizer(radius, radius);
	}


	void Circle::Render()
	{
		Renderer::DrawCircle(shape->GetCorners(), color);
	}


	bool Circle::Save(FileSaver& fs)
	{
		RenderableGameObject::Save(fs);
		fs.Write(radius);
		fs.Write(color);
		return fs.GetStatus();
	}


	bool Circle::Load(FileLoader& fl)
	{
		RenderableGameObject::Load(fl);
		fl.Read(radius);
		fl.Read(color);
		SetSizer(radius, radius);
		return fl.GetStatus();
	}


	void Circle::RenderSelection()
	{
		Renderer::DrawCircle(shape->GetCorners(), selectColor);
	}

}
#include "pch.h"
#include "Quad.hpp"
#include "Engine/Renderer/Renderer.hpp"


namespace Sharpheus {

	SPH_START_CLASSINFO(Quad, "quad.png")
		SPH_PROVIDE_UFLOAT(Quad, "Width", GetWidth, SetWidth)
		SPH_PROVIDE_UFLOAT(Quad, "Height", GetHeight, SetHeight)
		SPH_PROVIDE_COLOR(Quad, "Color", GetColor, SetColor)
	SPH_END_CLASSINFO


	Quad::Quad(GameObject* parent, const std::string& name) :
		ShapedGameObject(parent, name, new Rect()) {}


	void Quad::CopyFrom(GameObject* other)
	{
		SPH_CHECKTYPE(other, Quad);

		ShapedGameObject::CopyFrom(other);
		Quad* trueOther = (Quad*)other;
		width = trueOther->width;
		height = trueOther->height;
		color = trueOther->color;
		SetSizer(width, height);
	}


	void Quad::Render()
	{
		Renderer::DrawMonocromeQuad(shape->GetCorners(), color);
	}


	bool Quad::Save(FileSaver& fs)
	{
		ShapedGameObject::Save(fs);
		fs.Write(width);
		fs.Write(height);
		fs.Write(color);
		return fs.GetStatus();
	}


	bool Quad::Load(FileLoader& fl)
	{
		ShapedGameObject::Load(fl);
		fl.Read(width);
		fl.Read(height);
		fl.Read(color);
		SetSizer(width, height);
		return fl.GetStatus();
	}

}
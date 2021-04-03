#include "pch.h"
#include "Quad.hpp"
#include "Engine/Renderer/Renderer.hpp"


namespace Sharpheus {

	ClassInfo Quad::classInfo("Quad", "quad.png", {
		new UFloatProvider<Quad>("Width", SPH_BIND_GETTER(Quad::GetWidth), SPH_BIND_SETTER(Quad::SetWidth)),
		new UFloatProvider<Quad>("Height", SPH_BIND_GETTER(Quad::GetHeight), SPH_BIND_SETTER(Quad::SetHeight)),
		new ColorProvider<Quad>("Color", SPH_BIND_GETTER(Quad::GetColor), SPH_BIND_SETTER(Quad::SetColor))
		});


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
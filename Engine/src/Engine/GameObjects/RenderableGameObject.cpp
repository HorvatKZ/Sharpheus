#include "pch.h"
#include "RenderableGameObject.hpp"
#include "Engine/Level.hpp"


namespace Sharpheus {

	RenderableGameObject::RenderableGameObject(GameObject* parent, const std::string& name, Shape* shape)
		: ShapedGameObject(parent, name, shape)
	{
	}


	RenderableGameObject::~RenderableGameObject()
	{
		level->RemoveFromLayers(this);
	}


	void RenderableGameObject::CopyFrom(GameObject* other)
	{
		ShapedGameObject::CopyFrom(other);
		RenderableGameObject* trueOther = (RenderableGameObject*)other;
		SetLayer(trueOther->layer);
	}


	bool RenderableGameObject::SetLayer(const std::string& layer)
	{
		if (level != nullptr) {
			return level->AddToLayer(this, layer);
		}
		return false;
	}


	void RenderableGameObject::RenderAsSelected()
	{
		Render();
		RenderSelection();
	}


	bool RenderableGameObject::Load(FileLoader& fl)
	{
		ShapedGameObject::Load(fl);
		std::string newLayer;
		fl.Read(newLayer);
		SetLayer(newLayer);
		return fl.GetStatus();
	}


	bool RenderableGameObject::Save(FileSaver& fs)
	{
		ShapedGameObject::Save(fs);
		fs.Write(layer);
		return fs.GetStatus();
	}

}
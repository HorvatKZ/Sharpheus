#include "pch.h"
#include "Sprite.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"


namespace Sharpheus {

	SPH_START_CLASSINFO(Sprite, "sprite.png")
		SPH_PROVIDE_IMAGE(Sprite, "Image", GetImage, SetImage, SetImageFromPath)
		SPH_PROVIDE_COLOR(Sprite, "Tint", GetTint, SetTint)
	SPH_END_CLASSINFO


	Sprite::Sprite(GameObject* parent, const std::string& name) :
		ShapedGameObject(parent, name, new Rect()) {}


	void Sprite::CopyFrom(GameObject* other)
	{
		SPH_CHECKTYPE(other, Sprite);

		ShapedGameObject::CopyFrom(other);
		Sprite* trueOther = (Sprite*)other;
		SetImage(trueOther->image);
		tint = trueOther->tint;
	}


	void Sprite::SetImageFromPath(const std::string& path, bool filtered)
	{
		SetImage(ResourceManager::GetImage(path, filtered));
	}


	void Sprite::Render()
	{
		if (image != nullptr) {
			image->Render(shape->GetCorners(), tint);
		}
	}


	bool Sprite::Save(FileSaver& fs)
	{
		ShapedGameObject::Save(fs);
		fs.Write(image);
		fs.Write(tint);
		return fs.GetStatus();
	}


	bool Sprite::Load(FileLoader& fl)
	{
		ShapedGameObject::Load(fl);
		Image* img;
		fl.Read(&img);
		SetImage(img);
		fl.Read(tint);
		return fl.GetStatus();
	}

}
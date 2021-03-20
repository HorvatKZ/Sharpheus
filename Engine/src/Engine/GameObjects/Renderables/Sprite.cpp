#include "pch.h"
#include "Sprite.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"


namespace Sharpheus {

	ClassInfo Sprite::classInfo("Sprite", "sprite.png", {
		new ImageProvider<Sprite>("Image", SPH_BIND_GETTER(Sprite::GetImage), SPH_BIND_SETTER(Sprite::SetImage), SPH_BIND_3(Sprite::SetImageFromPath)),
		new ColorProvider<Sprite>("Tint", SPH_BIND_GETTER(Sprite::GetTint), SPH_BIND_SETTER(Sprite::SetTint))
	});


	Sprite::Sprite(GameObject* parent, const std::string& name) :
		ShapedGameObject(parent, name, new Rect()) {}


	void Sprite::CopyFrom(GameObject* other)
	{
		SPH_CHECKTYPE(other, Sprite);

		GameObject::CopyFrom(other);
		Sprite* trueOther = (Sprite*)other;
		SetImage(trueOther->image);
		tint = trueOther->tint;
	}


	void Sprite::SetImageFromPath(const std::string& path, bool filtered)
	{
		SetImage(ResourceManager::GetImage(path, filtered));
	}


	void Sprite::Tick(float deltaTime) {}


	void Sprite::Render()
	{
		if (image != nullptr) {
			image->Render(shape->GetCorners(), tint);
		}
	}


	bool Sprite::Save(FileSaver& fs)
	{
		GameObject::Save(fs);
		fs.Write(image);
		fs.Write(tint);
		return fs.GetStatus();
	}


	bool Sprite::Load(FileLoader& fl)
	{
		GameObject::Load(fl);
		Image* img;
		fl.Read(&img);
		SetImage(img);
		fl.Read(tint);
		return fl.GetStatus();
	}

}
#include "pch.h"
#include "Sprite.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"


namespace Sharpheus {

	ClassInfo Sprite::classInfo("Sprite", "sprite.png", {
		new ImageProvider<Sprite>("Image", SPH_BIND_GETTER(Sprite::GetImage), SPH_BIND_SETTER(Sprite::SetImage), SPH_BIND_3(Sprite::SetImageFromPath)),
		new ColorProvider<Sprite>("Tint", SPH_BIND_GETTER(Sprite::GetTint), SPH_BIND_SETTER(Sprite::SetTint))
	});


	SPH_IMPL_COPY(Sprite);

	Sprite::Sprite(GameObject* parent, const std::string& name) :
		RectGameObject(parent, name) {}


	void Sprite::SetImageFromPath(const std::string& path, bool filtered)
	{
		image = ResourceManager::GetImage(path, filtered);
		needToRecalcOffset = true;
	}


	void Sprite::Tick(float deltaTime) {}


	void Sprite::Render()
	{
		if (image != nullptr) {
			if (needToRecalcOffset) {
				RecalcOffsets();
			}

			image->Render(worldTrafo.pos + offsets[0], worldTrafo.pos + offsets[1], worldTrafo.pos + offsets[2],
				worldTrafo.pos + offsets[3], tint);
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
		fl.Read(&image);
		fl.Read(tint);
		needToRecalcOffset = true;
		return fl.GetStatus();
	}


	void Sprite::RecalcOffsets()
	{
		RecalcOffsetsCommon(image->GetWidth() * worldTrafo.scale.x, image->GetHeight() * worldTrafo.scale.y, worldTrafo.rot);
	}
}
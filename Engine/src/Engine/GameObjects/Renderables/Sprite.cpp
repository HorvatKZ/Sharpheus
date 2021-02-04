#include "pch.h"
#include "Sprite.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"


namespace Sharpheus {

	Sprite::Sprite(GameObject* parent, const std::string& name, const Transform& trafo, const std::string& imagePath) :
		GameObject(parent, name, trafo), image(ResourceManager::GetImage(imagePath, true)) {}

	Sprite::Sprite(GameObject* parent, const std::string& name, const Transform& trafo, Image* image) :
		GameObject(parent, name, trafo), image(image) {}


	void Sprite::SetTrafo(const Transform& trafo)
	{
		needToRecalcOffset = needToRecalcOffset || (this->trafo.scale != trafo.scale || this->trafo.rot != trafo.rot);
		GameObject::SetTrafo(trafo);
	}


	void Sprite::TickThis(float deltaTime) {}


	void Sprite::RenderThis()
	{
		if (needToRecalcOffset) {
			uint32_t imgWidth = image->GetWidth();
			uint32_t imgHeight = image->GetHeight();
			offset = Point(imgWidth * worldTrafo.scale.x / 2, imgHeight * worldTrafo.scale.y / 2).Rotate(worldTrafo.rot);
			secondaryOffset = Point(-(imgWidth * worldTrafo.scale.x / 2), imgHeight * worldTrafo.scale.y / 2).Rotate(worldTrafo.rot);
			needToRecalcOffset = false;
		}
		
		image->Render(worldTrafo.pos - offset, worldTrafo.pos - secondaryOffset, worldTrafo.pos + offset, worldTrafo.pos + secondaryOffset);
	}


	void Sprite::UpdateWorldTrafo(const Transform& parentWorldTrafo)
	{
		Point prevScale = worldTrafo.scale;
		float prevRot = worldTrafo.rot;
		GameObject::UpdateWorldTrafo(parentWorldTrafo);
		needToRecalcOffset = needToRecalcOffset || (prevScale != worldTrafo.scale || prevRot != worldTrafo.rot);
	}

}
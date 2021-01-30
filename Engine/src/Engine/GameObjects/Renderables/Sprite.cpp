#include "pch.h"
#include "Sprite.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"


namespace Sharpheus {

	Sprite::Sprite(const std::string& name, const Transform& trafo, const std::string& imagePath) :
		GameObject(name, trafo), image(ResourceManager::GetImage(imagePath, true)) {}

	Sprite::Sprite(const std::string& name, const Transform& trafo, Image* image) :
		GameObject(name, trafo), image(image) {}


	void Sprite::SetTrafo(const Transform& trafo)
	{
		GameObject::SetTrafo(trafo);
		needToRecalcOffset = true;
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
		GameObject::UpdateWorldTrafo(parentWorldTrafo);
		needToRecalcOffset = true;
	}

}
#pragma once

#include "../GameObject.hpp"
#include "Engine/ResourceManager/Image.hpp"


namespace Sharpheus {

	class SPH_EXPORT Sprite : public GameObject
	{
	public:
		Sprite(GameObject* parent, const std::string& name, const Transform& trafo, const std::string& imagePath);
		Sprite(GameObject* parent, const std::string& name, const Transform& trafo, Image* image);
		virtual ~Sprite() = default;

		virtual void SetTrafo(const Transform& trafo) override;

		SPH_DECL_GAMEOBJECT(Sprite)

	protected:
		Image* image;
		Point offset;
		Point secondaryOffset;
		bool needToRecalcOffset = true;

		virtual void TickThis(float deltaTime) override;
		virtual void RenderThis() override;

		virtual void UpdateWorldTrafo(const Transform& parentWorldTrafo) override;
	};

}
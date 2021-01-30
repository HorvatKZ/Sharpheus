#pragma once

#include "pch.h"
#include "../GameObject.hpp"
#include "Engine/ResourceManager/Image.hpp"


namespace Sharpheus {

	class SPH_EXPORT Sprite : public GameObject
	{
	public:
		Sprite(const std::string& name, const Transform& trafo, const std::string& imagePath);
		Sprite(const std::string& name, const Transform& trafo, Image* image);
		virtual ~Sprite() = default;

		virtual void SetTrafo(const Transform& trafo) override;

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
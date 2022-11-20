#pragma once

#include "../RenderableGameObject.hpp"
#include "Engine/ResourceManager/Image.hpp"


namespace Sharpheus {

	class SPH_EXPORT Circle : public RenderableGameObject
	{
	public:
		Circle(GameObject* parent, const std::string& name);
		virtual ~Circle() = default;
		virtual void CopyFrom(GameObject* other) override;

		inline float GetRadius() { return radius; }
		inline const Color& GetColor() { return color; }
		inline void SetRadius(float radius) { this->radius = radius; SetSizer(radius, radius); }
		inline void SetColor(const Color& color) { this->color = color; }

		virtual void Render() override;

		virtual bool Load(FileLoader& fl) override;

		SPH_DECL_GAMEOBJECT(Circle)

	protected:
		float radius = 0.f;
		Color color = Color::White;

		virtual bool Save(FileSaver& fs) override;

		virtual void RenderSelection() override;
	};

}
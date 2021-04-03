#pragma once

#include "../ShapedGameObject.hpp"
#include "Engine/ResourceManager/Image.hpp"


namespace Sharpheus {

	class SPH_EXPORT Quad : public ShapedGameObject
	{
	public:
		Quad(GameObject* parent, const std::string& name);
		virtual ~Quad() = default;
		virtual void CopyFrom(GameObject* other) override;

		inline float GetWidth() { return width; }
		inline float GetHeight() { return height; }
		inline const Color& GetColor() { return color; }
		inline void SetWidth(float width) { this->width = width; SetSizer(width, height); }
		inline void SetHeight(float height) { this->height = height; SetSizer(width, height); }
		inline void SetColor(const Color& color) { this->color = color; }

		virtual bool Load(FileLoader& fl) override;

		SPH_DECL_GAMEOBJECT(Quad)

	protected:
		float width = 0.f, height = 0.f;
		Color color = Color::White;

		virtual bool Save(FileSaver& fs) override;

		virtual void Render() override;
	};

}
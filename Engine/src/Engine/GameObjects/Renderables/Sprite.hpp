#pragma once

#include "../RectGameObject.hpp"
#include "Engine/ResourceManager/Image.hpp"


namespace Sharpheus {

	class SPH_EXPORT Sprite : public RectGameObject
	{
	public:
		Sprite(GameObject* parent, const std::string& name);
		virtual ~Sprite() = default;

		inline Image* GetImage() { return image; }
		inline const Color& GetTint() { return tint; }
		inline void SetImage(Image* image) { this->image = image; needToRecalcOffset = true; }
		inline void SetTint(const Color& tint) { this->tint = tint; }
		void SetImageFromPath(const std::string& path, bool filtered);

		virtual bool Load(FileLoader& fl) override;

		SPH_DECL_GAMEOBJECT(Sprite)

	protected:
		Image* image = nullptr;
		Color tint = Color::White;

		virtual bool Save(FileSaver& fs) override;

		virtual void RecalcOffsets() override;
		virtual void Tick(float deltaTime) override;
		virtual void Render() override;
	};

}
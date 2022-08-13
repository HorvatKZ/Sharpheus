#pragma once

#include "../RenderableGameObject.hpp"
#include "Engine/ResourceManager/Image.hpp"


namespace Sharpheus {

	class SPH_EXPORT Sprite : public RenderableGameObject
	{
	public:
		Sprite(GameObject* parent, const std::string& name);
		virtual ~Sprite() = default;
		virtual void CopyFrom(GameObject * other) override;

		inline const Image* GetImage() { return image; }
		inline const Color& GetTint() { return tint; }
		inline void SetTint(const Color& tint) { this->tint = tint; }
		inline void SetImage(const Image* image) {
			this->image = image;
			if (image != nullptr) {
				SetSizer(image->GetWidth(), image->GetHeight());
			} else {
				SetSizer(0, 0);
			}
		}

		void SetImageFromPath(const std::string& path, bool filtered);

		virtual void Render() override;

		virtual bool Load(FileLoader& fl) override;

		SPH_DECL_GAMEOBJECT(Sprite)

	protected:
		const Image* image = nullptr;
		Color tint = Color::White;

		virtual bool Save(FileSaver& fs) override;
	};

}
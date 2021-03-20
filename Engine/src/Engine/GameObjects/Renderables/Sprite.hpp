#pragma once

#include "../ShapedGameObject.hpp"
#include "Engine/ResourceManager/Image.hpp"


namespace Sharpheus {

	class SPH_EXPORT Sprite : public ShapedGameObject
	{
	public:
		Sprite(GameObject* parent, const std::string& name);
		virtual ~Sprite() = default;
		virtual void CopyFrom(GameObject * other) override;

		inline Image* GetImage() { return image; }
		inline const Color& GetTint() { return tint; }
		inline void SetTint(const Color& tint) { this->tint = tint; }
		inline void SetImage(Image* image) {
			this->image = image;
			if (image != nullptr) {
				SetSizer(image->GetWidth(), image->GetHeight());
			}
		}

		void SetImageFromPath(const std::string& path, bool filtered);

		virtual bool Load(FileLoader& fl) override;

		SPH_DECL_GAMEOBJECT(Sprite)

	protected:
		Image* image = nullptr;
		Color tint = Color::White;

		virtual bool Save(FileSaver& fs) override;

		virtual void Tick(float deltaTime) override;
		virtual void Render() override;
	};

}
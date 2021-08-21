#pragma once

#include "../RenderableGameObject.hpp"
#include "Engine/ResourceManager/Animation.hpp"


namespace Sharpheus {

	class SPH_EXPORT AnimatedSprite : public RenderableGameObject
	{
	public:
		AnimatedSprite(GameObject* parent, const std::string& name);
		virtual ~AnimatedSprite() = default;
		virtual void CopyFrom(GameObject* other) override;

		inline Animation* GetAnimation() { return anim; }
		inline const Color& GetTint() { return tint; }
		inline float GetSpeed() { return speed; }
		inline void SetTint(const Color& tint) { this->tint = tint; }
		inline void SetSpeed(float speed) { this->speed = speed; }
		inline void SetAnimation(Animation* anim) {
			this->anim = anim;
			currTime = 0.f;
			if (anim != nullptr) {
				SetSizer(anim->GetFrameWidth(), anim->GetFrameHeight());
			} else {
				SetSizer(0, 0);
			}
		}

		void SetAnimationFromPath(const std::string& path);

		virtual void Render() override;

		virtual bool Load(FileLoader& fl) override;

		SPH_DECL_GAMEOBJECT(AnimatedSprite)

	protected:
		Animation* anim = nullptr;
		Color tint = Color::White;
		float speed = 1.f;
		float currTime = 0.f;

		virtual bool Save(FileSaver& fs) override;

		virtual void Tick(float deltaTime) override;
	};

}
#pragma once

#include "../ShapedGameObject.hpp"
#include "Engine/ResourceManager/Animation.hpp"


namespace Sharpheus {

	class SPH_EXPORT AnimationPlayer : public ShapedGameObject
	{
	public:
		AnimationPlayer(GameObject* parent, const std::string& name);
		virtual ~AnimationPlayer() = default;
		virtual void CopyFrom(GameObject* other) override;

		inline const Color& GetTint() { return tint; }
		inline float GetSpeed() { return speed; }
		inline void SetTint(const Color& tint) { this->tint = tint; }
		inline void SetSpeed(float speed) { this->speed = speed; }

		inline uint32_t GetAnimationCount() { return anims.size(); }
		inline Animation* GetAnimation(uint32_t ind) {
			if (ind < anims.size()) {
				return anims[ind];
			}
			return nullptr;
		}
		inline const std::string& GetAnimationName(uint32_t ind) {
			Animation* anim = GetAnimation(ind);
			if (anim == nullptr) {
				return "";
			}
			return anim->GetName();
		}

		inline void SetAnimation(uint32_t ind, Animation* anim) {
			if (ind < anims.size()) {
				anims[ind] = anim;
			}
		}
		inline void AddAnimation(Animation* anim) {
			anims.push_back(anim);
			SetCurrent(anims.size() - 1);
		}
		inline void RemoveAnimationByIndex(uint32_t ind) {
			if (ind < anims.size()) {
				anims.erase(anims.begin() + ind);
				if (currAnimInd != 0 && ind <= currAnimInd) {
					--currAnimInd;
				}
			}
		}
		inline void RemoveAnimation(Animation* anim) {
			uint32_t i = 0;
			while (i < anims.size() && anims[i] != anim) {
				++i;
			}
			RemoveAnimationByIndex(i);
		}

		void SetAnimationFromPath(uint32_t ind, const std::string& path);
		void AddAnimationFromPath(const std::string& path);

		inline uint32_t GetCurrent() { return currAnimInd; }
		inline void SetCurrent(uint32_t ind) {
			if (ind < anims.size() && currAnimInd != ind) {
				currAnimInd = ind;
				currTime = 0.f;
				Animation* anim = anims[ind];
				SetSizer(anim->GetFrameWidth(), anim->GetFrameHeight());
			}
		}
		inline void SetCurrentByName(const std::string& name) {
			for (uint32_t i = 0; i < anims.size(); ++i) {
				if (anims[i]->GetName() == name) {
					return SetCurrent(i);
				}
			}
		}

		virtual bool Load(FileLoader& fl) override;

		SPH_DECL_GAMEOBJECT(AnimationPlayer)

	protected:
		std::vector<Animation*> anims;
		Color tint = Color::White;
		float speed = 1.f;
		float currTime = 0.f;
		uint32_t currAnimInd = 0;

		virtual bool Save(FileSaver& fs) override;

		virtual void Tick(float deltaTime) override;
		virtual void Render() override;
	};

}
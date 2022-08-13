#pragma once

#include "../RenderableGameObject.hpp"
#include "Engine/ResourceManager/Animation.hpp"


namespace Sharpheus {

	class SPH_EXPORT AnimationPlayer : public RenderableGameObject
	{
	public:
		AnimationPlayer(GameObject* parent, const std::string& name);
		virtual ~AnimationPlayer() = default;
		virtual void CopyFrom(GameObject* other) override;

		inline const Color& GetTint() { return tint; }
		inline float GetSpeed() { return speed; }
		inline void SetTint(const Color& tint) { this->tint = tint; }
		inline void SetSpeed(float speed) { this->speed = speed; }

		inline uint32 GetAnimationCount() { return anims.size(); }
		inline const Animation* GetAnimation(uint32 ind) {
			if (ind < anims.size()) {
				return anims[ind];
			}
			return nullptr;
		}
		inline const std::string& GetAnimationName(uint32 ind) {
			const Animation* anim = GetAnimation(ind);
			if (anim == nullptr) {
				return "";
			}
			return anim->GetName();
		}

		inline bool SetAnimation(uint32 ind, const Animation* anim) {
			if (ind < anims.size()) {
				anims[ind] = anim;
				return true;
			}
			return false;
		}
		inline bool AddAnimation(const Animation* anim) {
			if (anim == nullptr) {
				return false;
			}
			anims.push_back(anim);
			SetCurrent(anims.size() - 1);
			return true;
		}
		inline bool RemoveAnimationByIndex(uint32 ind) {
			if (ind < anims.size()) {
				anims.erase(anims.begin() + ind);
				if (currAnimInd != 0 && ind <= currAnimInd) {
					--currAnimInd;
				}
				return true;
			}
			return false;
		}
		inline bool RemoveAnimation(const Animation* anim) {
			uint32 i = 0;
			while (i < anims.size() && anims[i] != anim) {
				++i;
			}
			return RemoveAnimationByIndex(i);
		}

		inline const Animation* GetCurrentAnimation() { return GetAnimation(GetCurrent()); }
		inline const std::string& GetCurrentAnimationName() { return GetAnimationName(GetCurrent()); }

		bool SetAnimationFromPath(uint32 ind, const std::string& path);
		bool AddAnimationFromPath(const std::string& path);

		inline uint32 GetCurrent() { return currAnimInd; }
		inline bool SetCurrent(uint32 ind) {
			if (ind < anims.size() && currAnimInd != ind) {
				if (playOnceActive) {
					fallBackInd = ind;
				}
				else {
					currAnimInd = ind;
					currTime = 0.f;
					const Animation* anim = anims[ind];
					SetSizer(anim->GetFrameWidth(), anim->GetFrameHeight());
				}
				return true;
			}
			return false;
		}
		inline bool SetCurrentByName(const std::string& name) {
			for (uint32 i = 0; i < anims.size(); ++i) {
				if (anims[i]->GetName() == name) {
					return SetCurrent(i);
				}
			}
			return false;
		}

		inline void PlayOnce(uint32 ind) {
			if (!playOnceActive) {
				fallBackInd = currAnimInd;
				SetCurrent(ind);
				playOnceActive = true;
			}
		}
		inline void PlayOnce(const std::string& name) {
			if (!playOnceActive) {
				fallBackInd = currAnimInd;
				SetCurrentByName(name);
				playOnceActive = true;
			}
		}

		virtual void Render() override;

		virtual bool Load(FileLoader& fl) override;

		SPH_DECL_GAMEOBJECT(AnimationPlayer)

	protected:
		std::vector<const Animation*> anims;
		Color tint = Color::White;
		float speed = 1.f;
		float currTime = 0.f;
		uint32 currAnimInd = 0, fallBackInd = 0;
		bool playOnceActive = false;

		virtual bool Save(FileSaver& fs) override;

		virtual void Tick(float deltaTime) override;

		void FallBack();
	};

}
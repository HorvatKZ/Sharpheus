#include "pch.h"
#include "AnimationPlayer.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"


namespace Sharpheus {

	ClassInfo AnimationPlayer::classInfo("AnimationPlayer", "animplayer.png", {
		new StringListProvider<AnimationPlayer>("Animations", SPH_BIND_1(AnimationPlayer::GetAnimationCount), SPH_BIND_1(AnimationPlayer::GetCurrent), 
			SPH_BIND_2(AnimationPlayer::GetAnimationName), SPH_BIND_2(AnimationPlayer::SetCurrent), SPH_BIND_3(AnimationPlayer::SetAnimationFromPath),
			SPH_BIND_2(AnimationPlayer::AddAnimationFromPath), SPH_BIND_2(AnimationPlayer::RemoveAnimationByIndex)),
		new ColorProvider<AnimationPlayer>("Tint", SPH_BIND_GETTER(AnimationPlayer::GetTint), SPH_BIND_SETTER(AnimationPlayer::SetTint)),
		new FloatProvider<AnimationPlayer>("Speed", SPH_BIND_GETTER(AnimationPlayer::GetSpeed), SPH_BIND_SETTER(AnimationPlayer::SetSpeed))
		});


	AnimationPlayer::AnimationPlayer(GameObject* parent, const std::string& name) :
		ShapedGameObject(parent, name, new Rect()) {}


	void AnimationPlayer::CopyFrom(GameObject* other)
	{
		SPH_CHECKTYPE(other, AnimationPlayer);

		ShapedGameObject::CopyFrom(other);
		AnimationPlayer* trueOther = (AnimationPlayer*)other;
		SetCurrent(trueOther->currAnimInd);
		anims.clear();
		for (uint32_t i = 0; i < trueOther->GetAnimationCount(); ++i) {
			AddAnimation(trueOther->GetAnimation(i));
		}
		tint = trueOther->tint;
	}


	void AnimationPlayer::SetAnimationFromPath(uint32_t ind, const std::string& path)
	{
		SetAnimation(ind, ResourceManager::GetAnimation(path));
	}


	void AnimationPlayer::AddAnimationFromPath(const std::string& path)
	{
		AddAnimation(ResourceManager::GetAnimation(path));
	}


	void AnimationPlayer::Tick(float deltaTime)
	{
		Animation* anim = GetAnimation(currAnimInd);
		if (anim != nullptr) {
			currTime += deltaTime * speed;
			float animFullTime = anim->GetFullTime();
			while (currTime > animFullTime) {
				if (playOnceActive) {
					FallBack();
				}
				else {
					currTime -= animFullTime;
				}
			}
			while (currTime < 0.f) {
				if (playOnceActive) {
					FallBack();
				}
				else {
					currTime += animFullTime;
				}
			}
		}
	}


	void AnimationPlayer::Render()
	{
		if (currAnimInd < anims.size()) {
			GetAnimation(currAnimInd)->Render(shape->GetCorners(), currTime, tint);
		}
	}


	void AnimationPlayer::FallBack()
	{
		playOnceActive = false;
		SetCurrent(fallBackInd);
		fallBackInd = 0;
	}


	bool AnimationPlayer::Save(FileSaver& fs)
	{
		ShapedGameObject::Save(fs);
		fs.Write((uint32_t)anims.size());
		for (uint32_t i = 0; i < anims.size(); ++i) {
			fs.Write(anims[i]);
		}
		fs.Write(currAnimInd);
		fs.Write(tint);
		fs.Write(speed);
		return fs.GetStatus();
	}


	bool AnimationPlayer::Load(FileLoader& fl)
	{
		ShapedGameObject::Load(fl);
		uint32_t animCount = 0;
		fl.Read(animCount);
		for (uint32_t i = 0; i < animCount; ++i) {
			Animation* anim;
			fl.Read(&anim);
			AddAnimation(anim);
		}
		uint32_t curr = 0;
		fl.Read(curr);
		SetCurrent(curr);
		fl.Read(tint);
		fl.Read(speed);
		return fl.GetStatus();
	}

}
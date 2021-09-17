#include "pch.h"
#include "AnimationPlayer.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"


namespace Sharpheus {

	SPH_START_CLASSINFO(AnimationPlayer, "animplayer.png")
		SPH_PROVIDE_LAYER(AnimationPlayer, "Layer", GetLayer, SetLayer)
		SPH_PROVIDE_STRINGLIST_CURR(AnimationPlayer, "Animations", ANIM, GetAnimationCount, GetCurrent, GetAnimationName, SetCurrent,
			SetAnimationFromPath, AddAnimationFromPath, RemoveAnimationByIndex)
		SPH_PROVIDE_COLOR(AnimationPlayer, "Tint", GetTint, SetTint)
		SPH_PROVIDE_FLOAT(AnimationPlayer, "Speed", GetSpeed, SetSpeed)
	SPH_END_CLASSINFO


	AnimationPlayer::AnimationPlayer(GameObject* parent, const std::string& name) :
		RenderableGameObject(parent, name, new Rect()) {}


	void AnimationPlayer::CopyFrom(GameObject* other)
	{
		SPH_CHECKTYPE(other, AnimationPlayer);

		RenderableGameObject::CopyFrom(other);
		AnimationPlayer* trueOther = (AnimationPlayer*)other;
		SetCurrent(trueOther->currAnimInd);
		anims.clear();
		for (uint32 i = 0; i < trueOther->GetAnimationCount(); ++i) {
			AddAnimation(trueOther->GetAnimation(i));
		}
		tint = trueOther->tint;
	}


	bool AnimationPlayer::SetAnimationFromPath(uint32 ind, const std::string& path)
	{
		return SetAnimation(ind, ResourceManager::GetAnimation(path));
	}


	bool AnimationPlayer::AddAnimationFromPath(const std::string& path)
	{
		return AddAnimation(ResourceManager::GetAnimation(path));
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
		RenderableGameObject::Save(fs);
		fs.Write((uint32)anims.size());
		for (uint32 i = 0; i < anims.size(); ++i) {
			fs.Write(anims[i]);
		}
		fs.Write(currAnimInd);
		fs.Write(tint);
		fs.Write(speed);
		return fs.GetStatus();
	}


	bool AnimationPlayer::Load(FileLoader& fl)
	{
		RenderableGameObject::Load(fl);
		uint32 animCount = 0;
		fl.Read(animCount);
		for (uint32 i = 0; i < animCount; ++i) {
			Animation* anim;
			fl.Read(&anim);
			AddAnimation(anim);
		}
		uint32 curr = 0;
		fl.Read(curr);
		SetCurrent(curr);
		fl.Read(tint);
		fl.Read(speed);
		return fl.GetStatus();
	}

}
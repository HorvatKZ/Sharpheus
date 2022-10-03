#include "pch.h"
#include "AnimatedSprite.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"


namespace Sharpheus {

	SPH_START_CLASSINFO(AnimatedSprite, "animsprite.png")
		SPH_PROVIDE_LAYER(AnimatedSprite, "Layer", GetLayer, SetLayer)
		SPH_PROVIDE_ANIM(AnimatedSprite, "Animation", GetAnimation, SetAnimation, SetAnimationFromPath)
		SPH_PROVIDE_COLOR(AnimatedSprite, "Tint", GetTint, SetTint)
		SPH_PROVIDE_FLOAT(AnimatedSprite, "Speed", GetSpeed, SetSpeed)
	SPH_END_CLASSINFO


	AnimatedSprite::AnimatedSprite(GameObject* parent, const std::string& name) :
		RenderableGameObject(parent, name, new Rect()) {}


	void AnimatedSprite::CopyFrom(GameObject* other)
	{
		SPH_CHECKTYPE(other, AnimatedSprite);

		RenderableGameObject::CopyFrom(other);
		AnimatedSprite* trueOther = (AnimatedSprite*)other;
		SetAnimation(trueOther->anim);
		tint = trueOther->tint;
	}


	void AnimatedSprite::SetAnimationFromPath(const std::string& path)
	{
		SetAnimation(ResourceManager::GetAnimation(path));
	}


	void AnimatedSprite::Tick(float deltaTime)
	{
		if (anim != nullptr) {
			currTime += deltaTime * speed;
			float animFullTime = anim->GetFullTime();
			if (currTime > animFullTime) {
				currTime -= animFullTime;
			} else if (currTime < 0.f) {
				currTime += animFullTime;
			}
		}
	}


	void AnimatedSprite::Render()
	{
		if (anim != nullptr) {
			anim->Render(shape->GetCorners(), currTime, shape->NeedToMirrorX(), tint);
		}
	}


	bool AnimatedSprite::Save(FileSaver& fs)
	{
		RenderableGameObject::Save(fs);
		fs.Write(anim);
		fs.Write(tint);
		fs.Write(speed);
		return fs.GetStatus();
	}


	bool AnimatedSprite::Load(FileLoader& fl)
	{
		RenderableGameObject::Load(fl);
		const Animation* anim;
		fl.Read(&anim);
		SetAnimation(anim);
		fl.Read(tint);
		fl.Read(speed);
		return fl.GetStatus();
	}

}
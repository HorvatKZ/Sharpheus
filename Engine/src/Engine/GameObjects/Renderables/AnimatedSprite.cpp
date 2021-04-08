#include "pch.h"
#include "AnimatedSprite.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"


namespace Sharpheus {

	ClassInfo AnimatedSprite::classInfo("AnimatedSprite", "animsprite.png", {
		new AnimationProvider<AnimatedSprite>("Animation", SPH_BIND_GETTER(AnimatedSprite::GetAnimation), SPH_BIND_SETTER(AnimatedSprite::SetAnimation), SPH_BIND_2(AnimatedSprite::SetAnimationFromPath)),
		new ColorProvider<AnimatedSprite>("Tint", SPH_BIND_GETTER(AnimatedSprite::GetTint), SPH_BIND_SETTER(AnimatedSprite::SetTint)),
		new FloatProvider<AnimatedSprite>("Speed", SPH_BIND_GETTER(AnimatedSprite::GetSpeed), SPH_BIND_SETTER(AnimatedSprite::SetSpeed))
	});


	AnimatedSprite::AnimatedSprite(GameObject* parent, const std::string& name) :
		ShapedGameObject(parent, name, new Rect()) {}


	void AnimatedSprite::CopyFrom(GameObject* other)
	{
		SPH_CHECKTYPE(other, AnimatedSprite);

		ShapedGameObject::CopyFrom(other);
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
			anim->Render(shape->GetCorners(), currTime, tint);
		}
	}


	bool AnimatedSprite::Save(FileSaver& fs)
	{
		ShapedGameObject::Save(fs);
		fs.Write(anim);
		fs.Write(tint);
		fs.Write(speed);
		return fs.GetStatus();
	}


	bool AnimatedSprite::Load(FileLoader& fl)
	{
		ShapedGameObject::Load(fl);
		Animation* anim;
		fl.Read(&anim);
		SetAnimation(anim);
		fl.Read(tint);
		fl.Read(speed);
		return fl.GetStatus();
	}

}
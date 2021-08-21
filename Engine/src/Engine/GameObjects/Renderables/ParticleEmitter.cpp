#include "pch.h"
#include "ParticleEmitter.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"


namespace Sharpheus {

	SPH_START_CLASSINFO(ParticleEmitter, "particleemitter.png")
		SPH_PROVIDE_LAYER(ParticleEmitter, "Layer", GetLayer, SetLayer)
		SPH_PROVIDE_IMAGE(ParticleEmitter, "Particle", GetParticle, SetParticle, SetParticleByPath)
		SPH_PROVIDE_POINT(ParticleEmitter, "Particle scale", GetParticleScale, SetParticleScale)
		SPH_PROVIDE_COLOR(ParticleEmitter, "Particle tint", GetTint, SetTint)
		SPH_PROVIDE_UFLOAT(ParticleEmitter, "Emition time", GetEmitionTime, SetEmitionTime)
		SPH_PROVIDE_UFLOAT(ParticleEmitter, "Lifetime", GetLifeTime, SetLifeTime)
		SPH_PROVIDE_UFLOAT(ParticleEmitter, "Speed", GetSpeed, SetSpeed)
		SPH_PROVIDE_UFLOAT(ParticleEmitter, "Gravity", GetGravity, SetGravity)
		SPH_PROVIDE_UFLOAT_RANGE(ParticleEmitter, "Friction", GetFriction, SetFriction, 0.f, 1.f)
		SPH_PROVIDE_BOOL(ParticleEmitter, "Shrink", IsShrink, SetShrink)
		SPH_PROVIDE_BOOL(ParticleEmitter, "Fade", IsFade, SetFade)
		SPH_PROVIDE_BOOL(ParticleEmitter, "Burst", IsBurst, SetBurst)
		SPH_PROVIDE_UINT(ParticleEmitter, "Burst N", GetBurstN, SetBurstN)
	SPH_END_CLASSINFO


	ParticleEmitter::ParticleEmitter(GameObject* parent, const std::string& name)
		: AxisGameObject(parent, name, new Oval())
	{
	}


	ParticleEmitter::~ParticleEmitter()
	{
	}


	void ParticleEmitter::CopyFrom(GameObject* other)
	{
		SPH_CHECKTYPE(other, ParticleEmitter);

		AxisGameObject::CopyFrom(other);
		ParticleEmitter* trueOther = (ParticleEmitter*)other;
		SetParticle(trueOther->particle);
		scale = trueOther->scale;
		tint = trueOther->tint;
		shrink = trueOther->shrink;
		fade = trueOther->fade;
		burst = trueOther->burst;
		lifeTime = trueOther->lifeTime;
		speed = trueOther->speed;
		emitionTime = trueOther->emitionTime;
		gravity = trueOther->gravity;
		friction = trueOther->friction;
		burstN = trueOther->burstN;
	}


	void ParticleEmitter::SetParticleByPath(const std::string& path, bool filtered)
	{
		SetParticle(ResourceManager::GetImage(path, filtered));
	}


	bool ParticleEmitter::Load(FileLoader& fl)
	{
		AxisGameObject::Load(fl);
		fl.Read(&particle);
		fl.Read(scale);
		fl.Read(tint);
		fl.Read(shrink);
		fl.Read(fade);
		fl.Read(burst);
		fl.Read(lifeTime);
		fl.Read(speed);
		fl.Read(emitionTime);
		fl.Read(gravity);
		fl.Read(friction);
		if (burst) {
			fl.Read(burstN);
		}

		UpdateSizer();
		return fl.GetStatus();
	}


	bool ParticleEmitter::Save(FileSaver& fs)
	{
		AxisGameObject::Save(fs);
		fs.Write(particle);
		fs.Write(scale);
		fs.Write(tint);
		fs.Write(shrink);
		fs.Write(fade);
		fs.Write(burst);
		fs.Write(lifeTime);
		fs.Write(speed);
		fs.Write(emitionTime);
		fs.Write(gravity);
		fs.Write(friction);
		if (burst) {
			fs.Write(burstN);
		}

		return fs.GetStatus();
	}


	void ParticleEmitter::Tick(float deltaTime)
	{
		totalTime += deltaTime;
		if (totalTime > lastEmit + emitionTime) {
			Emit((totalTime - lastEmit) / emitionTime * (burst ? burstN : 1));
			lastEmit = totalTime;
		}

		while (!particles.empty() && particles.front().startTime + lifeTime < totalTime) {
			particles.pop_front();
		}

		float frictionMult = 1.f - friction * deltaTime;
		for (auto it = particles.begin(); it != particles.end(); ++it) {
			if (hasCustomFunc) {
				customFunc(this, *it, deltaTime, totalTime - it->startTime);
			} else {
				it->pos += it->direction * it->speed * deltaTime;
			}
			it->direction += gravity * deltaTime / it->speed * Point::Down;
			it->speed *= frictionMult;
		}
	}


	void ParticleEmitter::Render()
	{
		if (particle != nullptr) {
			for (auto it = particles.begin(); it != particles.end(); ++it) {
				float lifeTimePercent = 1.f - ((totalTime - it->startTime) / lifeTime);
				Color fadedTint = tint;
				if (fade) {
					fadedTint.MakeOpaque(lifeTimePercent);
				}
				particle->Render(GetCorners(it->pos, shrink ? lifeTimePercent : 1.f), fadedTint);
			}
		}
	}


	void ParticleEmitter::RenderSelection()
	{
		GameObject::RenderSelection();
	}


	bool ParticleEmitter::IsSelected(const Point& pos)
	{
		return GameObject::IsSelected(pos);
	}


	void ParticleEmitter::Emit(uint32 n)
	{
		for (uint32 i = 0; i < n; ++i) {
			particles.push_back(ParticleState(GetRandDir(), worldTrafo.pos, totalTime, speed));
		}
	}

}
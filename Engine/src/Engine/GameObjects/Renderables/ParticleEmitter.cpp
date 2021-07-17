#include "pch.h"
#include "ParticleEmitter.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"


namespace Sharpheus {

	ClassInfo ParticleEmitter::classInfo("ParticleEmitter", "particleemitter.png", {
		new ImageProvider<ParticleEmitter>("Particle", SPH_BIND_GETTER(ParticleEmitter::GetParticle), SPH_BIND_SETTER(ParticleEmitter::SetParticle), SPH_BIND_3(ParticleEmitter::SetParticleByPath)),
		new PointProvider<ParticleEmitter>("Particle scale", SPH_BIND_GETTER(ParticleEmitter::GetParticleScale), SPH_BIND_SETTER(ParticleEmitter::SetParticleScale)),
		new ColorProvider<ParticleEmitter>("Particle tint", SPH_BIND_GETTER(ParticleEmitter::GetTint), SPH_BIND_SETTER(ParticleEmitter::SetTint)),
		new UFloatProvider<ParticleEmitter>("Emition time", SPH_BIND_GETTER(ParticleEmitter::GetEmitionTime), SPH_BIND_SETTER(ParticleEmitter::SetEmitionTime)),
		new UFloatProvider<ParticleEmitter>("Lifetime", SPH_BIND_GETTER(ParticleEmitter::GetLifeTime), SPH_BIND_SETTER(ParticleEmitter::SetLifeTime)),
		new UFloatProvider<ParticleEmitter>("Speed", SPH_BIND_GETTER(ParticleEmitter::GetSpeed), SPH_BIND_SETTER(ParticleEmitter::SetSpeed)),
		new UFloatProvider<ParticleEmitter>("Gravity", SPH_BIND_GETTER(ParticleEmitter::GetGravity), SPH_BIND_SETTER(ParticleEmitter::SetGravity)),
		new UFloatProvider<ParticleEmitter>("Friction", SPH_BIND_GETTER(ParticleEmitter::GetFriction), SPH_BIND_SETTER(ParticleEmitter::SetFriction), 0.f, 1.f),
		new BoolProvider<ParticleEmitter>("Shrink", SPH_BIND_GETTER(ParticleEmitter::IsShrink), SPH_BIND_SETTER(ParticleEmitter::SetShrink)),
		new BoolProvider<ParticleEmitter>("Fade", SPH_BIND_GETTER(ParticleEmitter::IsFade), SPH_BIND_SETTER(ParticleEmitter::SetFade)),
		new BoolProvider<ParticleEmitter>("Burst", SPH_BIND_GETTER(ParticleEmitter::IsBurst), SPH_BIND_SETTER(ParticleEmitter::SetBurst)),
		new UIntProvider<ParticleEmitter>("Burst N", SPH_BIND_GETTER(ParticleEmitter::GetBurstN), SPH_BIND_SETTER(ParticleEmitter::SetBurstN)),
	});


	ParticleEmitter::ParticleEmitter(GameObject* parent, const std::string& name)
		: AxisShapedGameObject(parent, name, new Oval())
	{
	}


	ParticleEmitter::~ParticleEmitter()
	{
	}


	void ParticleEmitter::CopyFrom(GameObject* other)
	{
		SPH_CHECKTYPE(other, ParticleEmitter);

		AxisShapedGameObject::CopyFrom(other);
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
		AxisShapedGameObject::Load(fl);
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

		return fl.GetStatus();
	}


	bool ParticleEmitter::Save(FileSaver& fs)
	{
		AxisShapedGameObject::Save(fs);
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


	void ParticleEmitter::Emit(uint32_t n)
	{
		for (uint32_t i = 0; i < n; ++i) {
			particles.push_back(ParticleState(GetRandDir(), worldTrafo.pos, totalTime, speed));
		}
	}

}
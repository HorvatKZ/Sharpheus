#pragma once

#include "../AxisGameObject.hpp"
#include "Engine/ResourceManager/Image.hpp"


namespace Sharpheus {

	class SPH_EXPORT ParticleEmitter : public AxisGameObject
	{
	public:
		struct ParticleState {
			Point direction, pos;
			float startTime, speed;

			ParticleState(const Point& direction, const Point& pos, float startTime, float speed)
				: direction(direction), pos(pos), startTime(startTime), speed(speed) {}
		};


		ParticleEmitter(GameObject* parent, const std::string& name);
		virtual ~ParticleEmitter();
		virtual void CopyFrom(GameObject* other) override;

		inline const Image* GetParticle() { return particle; }
		inline const Point& GetParticleScale() { return scale; }
		inline const Color& GetTint() { return tint; }
		inline bool IsShrink() { return shrink; }
		inline bool IsFade() { return fade; }
		inline bool IsBurst() { return burst; }
		inline uint32 GetBurstN() { return burstN; }
		inline float GetLifeTime() { return lifeTime; }
		inline float GetSpeed() { return speed; }
		inline float GetEmissionTime() { return emissionTime; }
		inline float GetGravity() { return gravity; }
		inline float GetFriction() { return friction; }

		inline void SetParticle(const Image* particle) { this->particle = particle; }
		inline void SetParticleScale(const Point& scale) { this->scale = scale; }
		inline void SetTint(const Color& tint) { this->tint = tint; }
		inline void SetShrink(bool shrink) { this->shrink = shrink; }
		inline void SetFade(bool fade) { this->fade = fade; }
		inline void SetBurst(bool burst) { this->burst = burst; }
		inline void SetBurstN(uint32 burstN) { this->burstN = burstN; }
		inline void SetLifeTime(float lifeTime) { this->lifeTime = lifeTime; }
		inline void SetSpeed(float speed) { this->speed = speed; }
		inline void SetEmissionTime(float emissionTime) { this->emissionTime = emissionTime; }
		inline void SetGravity(float gravity) { this->gravity = gravity; }
		inline void SetFriction(float friction) { this->friction = glm::clamp(friction, 0.f, 1.f); }

		void SetParticleByPath(const std::string& path, bool filtered);

		inline void SetCustomFunc(std::function<void(ParticleEmitter*, ParticleState&, float, float)>&& customFunc) {
			this->customFunc = std::move(customFunc);
			hasCustomFunc = true;
		}
		inline void ClearCustomFunc() {
			customFunc = nullptr;
			hasCustomFunc = false;
		}

		virtual void Render() override;

		virtual bool Load(FileLoader& fl) override;

		SPH_DECL_GAMEOBJECT(ParticleEmitter)
	
	protected:
		const Image* particle = nullptr;
		Point scale = Point(1.f, 1.f);
		Color tint = Color::White;
		bool shrink = false;
		bool fade = false;
		bool burst = false;
		float lifeTime = 1.f;
		float speed = 100.f;
		float emissionTime = 1.f;
		float gravity = 0.f;
		float friction = 0.f;
		uint32 burstN = 1;

		std::list<ParticleState> particles;
		std::function<void(ParticleEmitter*, ParticleState&, float, float)> customFunc = nullptr;
		bool hasCustomFunc = false;
		float totalTime = 0.f, lastEmit = 0.f;
		Point tempCorners[4];

		virtual bool Save(FileSaver& fs) override;

		virtual void Tick(float deltaTime) override;

		virtual void RenderSelection() override;
		virtual bool IsSelected(const Point& pos) override;
		void UpdateSizer() override {}

		void Emit(uint32 n);
		inline Point GetRandDir() {
			float x = ((float)rand() / RAND_MAX) * 2.f - 1.f;
			float y = ((float)rand() / RAND_MAX) * 2.f - 1.f;
			return Point(x, y).Normalize();
		}
		inline Point* GetCorners(const Point& center, float sizeModifier) {
			Point xShift = xAxis * particle->GetWidth() * scale.x * sizeModifier * 0.5f;
			Point yShift = yAxis * particle->GetHeight() * scale.y * sizeModifier * 0.5f;
			tempCorners[0] = center - xShift - yShift;
			tempCorners[1] = center + xShift - yShift;
			tempCorners[2] = center + xShift + yShift;
			tempCorners[3] = center - xShift + yShift;
			return tempCorners;
		}
	};

}
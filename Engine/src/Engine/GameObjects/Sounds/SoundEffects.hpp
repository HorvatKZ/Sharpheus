#pragma once

#include "../GameObject.hpp"
#include "Engine/ResourceManager/AudioPlayer.hpp"

namespace Sharpheus {

	class SPH_EXPORT SoundEffects : public GameObject
	{
	public:
		SoundEffects(GameObject* parent, const std::string& name);
		virtual ~SoundEffects();
		virtual void CopyFrom(GameObject* other) override;

		inline uint32 GetNumOfAudios() { return audios.size(); }
		inline const Audio* GetAudio(uint32 ind) { return audios[ind]; }
		inline const std::string& GetAudioName(uint32 ind) { return audios[ind]->GetName(); }
		inline bool SetAudio(uint32 ind, const Audio* audio) {
			if (ind >= audios.size() || audio == nullptr) {
				return false;
			}
			audios[ind] = audio;
			return true;
		}

		inline bool AddAudio(const Audio* audio) {
			if (audio == nullptr) {
				return false;
			}
			audios.push_back(audio);
			return true;
		}

		inline bool RemoveAudioByInd(uint32 ind) {
			if (ind >= audios.size()) {
				return false;
			}
			audios.erase(audios.begin() + ind);
			return true;
		}
		
		bool SetAudioFromPath(uint32 ind, const std::string& path);
		bool AddAudioFromPath(const std::string& path);
		bool RemoveAudio(const Audio* audio);

		void Play(uint32 ind, uint32 volume = 100, float playSpeed = 1.f);
		void Play(const std::string& name, uint32 volume = 100, float playSpeed = 1.f);
		void StopAll();

		virtual bool Load(FileLoader& fl) override;

		SPH_DECL_GAMEOBJECT(SoundEffects)

	protected:
		struct EffectInstance {
			SoLoud::handle handle;
			float endTime;

			EffectInstance(SoLoud::handle handle, float length);
		};

		struct EffectInstanceCompare {
			bool operator()(const EffectInstance& lhs, const EffectInstance& rhs) { return lhs.endTime > rhs.endTime; }
		};

		std::vector<const Audio*> audios;
		std::priority_queue<EffectInstance, std::vector<EffectInstance>, EffectInstanceCompare> activeHandles;

		virtual bool Save(FileSaver& fs) override;

		virtual void Tick(float deltaTime) override;

		inline uint32 GetIndOfAudio(const std::string& audioName) {
			uint32 i = 0;
			while (i < audios.size() && audios[i]->GetName() != audioName) {
				++i;
			}
			return i;
		}
	};

}
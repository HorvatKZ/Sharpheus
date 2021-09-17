#pragma once

#include "../GameObject.hpp"
#include "Engine/ResourceManager/AudioPlayer.hpp"

namespace Sharpheus {

	class SPH_EXPORT MusicPlayer : public GameObject
	{
	public:
		MusicPlayer(GameObject* parent, const std::string& name);
		virtual ~MusicPlayer();
		virtual void CopyFrom(GameObject* other) override;

		inline bool IsPlaying() { return isPlaying; }
		inline bool IsLooping() { return loop; }
		inline bool DoesStartByDefault() { return startByDefault; }
		inline uint32 GetVolume() { return volume; }
		inline float GetPlaySpeed() { return playSpeed; }
		inline Audio* GetMusic() { return music; }
		inline void SetLooping(bool loop) { this->loop = loop; }
		inline void SetStartByDefault(bool startByDefault) { this->startByDefault = startByDefault; }
		inline void SetMusic(Audio* music) { this->music = music; }
		void SetMusicFromPath(const std::string& musicPath);
		inline void SetVolume(uint32 volume) {
			this->volume = volume;
			if (isPlaying) {
				AudioPlayer::SetVolume(handle, volume);
			}
		}
		inline void SetPlaySpeed(float playSpeed) {
			this->playSpeed = playSpeed;
			if (isPlaying) {
				AudioPlayer::SetPlaySpeed(handle, playSpeed);
			}
		}

		void Play();
		void Stop();
		void Pause();
		void Resume();

		virtual bool Load(FileLoader& fl) override;

		SPH_DECL_GAMEOBJECT(MusicPlayer)

	protected:
		Audio* music = nullptr;
		bool wantToStart = true, isPlaying = false;
		bool loop = false, startByDefault = true;
		uint32 volume = 100;
		float playSpeed = 1.f;
		SoLoud::handle handle = 0;

		virtual bool Save(FileSaver& fs) override;

		virtual void Tick(float deltaTime) override;
	};

}
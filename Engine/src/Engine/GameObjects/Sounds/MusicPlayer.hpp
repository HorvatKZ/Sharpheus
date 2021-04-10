#pragma once

#include "../GameObject.hpp"
#include "Engine/ResourceManager/SoundPlayer.hpp"

namespace Sharpheus {

	class SPH_EXPORT MusicPlayer : public GameObject
	{
	public:
		MusicPlayer(GameObject* parent, const std::string& name);
		virtual ~MusicPlayer();
		virtual void CopyFrom(GameObject* other) override;

		inline bool IsPlaying() { return !musicPath.empty() && SoundPlayer::GetCurrent() == musicPath; }
		inline bool IsLooping() { return loop; }
		inline const std::string& GetMusicPath() { return musicPath; }
		inline void SetLooping(bool loop) { this->loop = loop; }
		inline void SetMusicPath(const std::string& musicPath) { this->musicPath = musicPath; }


		void Play();
		void Stop();

		virtual bool Load(FileLoader& fl) override;

		SPH_DECL_GAMEOBJECT(MusicPlayer)

	protected:
		std::string musicPath;
		bool wantToStart = true;
		bool loop = false;

		virtual bool Save(FileSaver& fs) override;

		virtual void Tick(float deltaTime) override;
	};

}
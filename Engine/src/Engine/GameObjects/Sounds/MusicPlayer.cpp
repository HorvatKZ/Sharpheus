#include "pch.h"
#include "MusicPlayer.hpp"
#include "Engine/ResourceManager/AudioPlayer.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"


namespace Sharpheus {

	SPH_START_CLASSINFO(MusicPlayer, "musicplayer.png")
		SPH_PROVIDE_AUDIO(MusicPlayer, "Music", GetMusic, SetMusic, SetMusicFromPath)
		SPH_PROVIDE_BOOL(MusicPlayer, "Default start", DoesStartByDefault, SetStartByDefault)
		SPH_PROVIDE_BOOL(MusicPlayer, "Looping", IsLooping, SetLooping)
		SPH_PROVIDE_UINT(MusicPlayer, "Volume", GetVolume, SetVolume)
		SPH_PROVIDE_UFLOAT(MusicPlayer, "Playspeed", GetPlaySpeed, SetPlaySpeed)
	SPH_END_CLASSINFO


	MusicPlayer::MusicPlayer(GameObject* parent, const std::string& name)
		: GameObject(parent, name)
	{
	}


	MusicPlayer::~MusicPlayer()
	{
		Stop();
	}


	void MusicPlayer::CopyFrom(GameObject* other)
	{
		SPH_CHECKTYPE(other, MusicPlayer);

		GameObject::CopyFrom(other);
		MusicPlayer* trueOther = (MusicPlayer*)other;
		music = trueOther->music;
		loop = trueOther->loop;
		startByDefault = trueOther->startByDefault;
		volume = trueOther->volume;
		playSpeed = trueOther->playSpeed;
		wantToStart = startByDefault;
	}


	void MusicPlayer::SetMusicFromPath(const std::string& musicPath)
	{
		SetMusic(ResourceManager::GetAudio(musicPath));
	}


	void MusicPlayer::Play()
	{
		wantToStart = true;
	}


	void MusicPlayer::Stop()
	{
		if (isPlaying) {
			AudioPlayer::Stop(handle);
			isPlaying = false;
		}
	}


	void MusicPlayer::Pause()
	{
		if (isPlaying) {
			AudioPlayer::Pause(handle);
		}
	}


	void MusicPlayer::Resume()
	{
		if (isPlaying) {
			AudioPlayer::Resume(handle);
		}
	}


	bool MusicPlayer::Load(FileLoader& fl)
	{
		GameObject::Load(fl);
		Audio* audio;
		fl.Read(&audio);
		SetMusic(audio);
		fl.Read(loop);
		fl.Read(startByDefault);
		fl.Read(volume);
		fl.Read(playSpeed);
		wantToStart = startByDefault;
		return fl.GetStatus();
	}


	bool MusicPlayer::Save(FileSaver& fs)
	{
		GameObject::Save(fs);
		fs.Write(music);
		fs.Write(loop);
		fs.Write(startByDefault);
		fs.Write(volume);
		fs.Write(playSpeed);
		return fs.GetStatus();
	}


	void MusicPlayer::Tick(float deltaTime)
	{
		if (wantToStart && music != nullptr) {
			handle = AudioPlayer::Play(music, loop, volume, playSpeed);
			wantToStart = false;
			isPlaying = true;
		}
	}

}
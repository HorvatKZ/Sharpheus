#include "pch.h"
#include "AudioPlayer.hpp"
#include "ResourceManager.hpp"


namespace Sharpheus {

	SoLoud::Soloud AudioPlayer::soundEngine;


	void AudioPlayer::Init()
	{
		soundEngine.init();
	}


	void AudioPlayer::Clear()
	{
		soundEngine.deinit();
	}


	SoLoud::handle AudioPlayer::Play(Audio* audio, bool looping, uint32 volume, float playSpeed)
	{
		SoLoud::handle handle = soundEngine.play(audio->GetAudio());
		soundEngine.setLooping(handle, looping);
		soundEngine.setVolume(handle, volume / 100.f);
		soundEngine.setRelativePlaySpeed(handle, playSpeed);
		return handle;
	}


	void AudioPlayer::Stop(SoLoud::handle handle)
	{
		soundEngine.stop(handle);
	}


	void AudioPlayer::StopAll()
	{
		soundEngine.stopAll();
	}


	void AudioPlayer::Pause(SoLoud::handle handle)
	{
		soundEngine.setPause(handle, true);
	}


	void AudioPlayer::Resume(SoLoud::handle handle)
	{
		soundEngine.setPause(handle, false);
	}


	void AudioPlayer::SetVolume(SoLoud::handle handle, uint32 volume)
	{
		soundEngine.setVolume(handle, volume / 100.f);
	}


	void AudioPlayer::SetPlaySpeed(SoLoud::handle handle, float speed)
	{
		soundEngine.setRelativePlaySpeed(handle, speed);
	}

}
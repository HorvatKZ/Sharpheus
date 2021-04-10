#include "pch.h"
#include "SoundPlayer.hpp"
#include "ResourceManager.hpp"
#include <windows.h>
#include <mmsystem.h>


namespace Sharpheus {

	std::string SoundPlayer::curr;


	void SoundPlayer::Init()
	{
	}

	void SoundPlayer::Clear()
	{
	}

	bool SoundPlayer::Play(const std::string& path, bool loop)
	{
		std::string fullPath = ResourceManager::PathToFullPath(path);

		DWORD flags = SND_ASYNC | SND_FILENAME;
		if (loop) {
			flags = flags | SND_LOOP;
		}

		if (PlaySoundA(fullPath.c_str(), NULL, flags)) {
			curr = path;
			return true;
		}

		return false;
	}

	void SoundPlayer::Stop(const std::string& path)
	{
		if (curr == path) {
			PlaySoundA(NULL, NULL, NULL);
		}
	}

}
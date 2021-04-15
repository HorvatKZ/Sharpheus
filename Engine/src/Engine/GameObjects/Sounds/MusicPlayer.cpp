#include "pch.h"
#include "MusicPlayer.hpp"
#include "Engine/ResourceManager/SoundPlayer.hpp"


namespace Sharpheus {

	ClassInfo MusicPlayer::classInfo("MusicPlayer", "musicplayer.png", {
		new SoundProvider<MusicPlayer>("Music", SPH_BIND_GETTER(MusicPlayer::GetMusicPath), SPH_BIND_SETTER(MusicPlayer::SetMusicPath)),
		new BoolProvider<MusicPlayer>("Default start", SPH_BIND_GETTER(MusicPlayer::DoesStartByDefault), SPH_BIND_SETTER(MusicPlayer::SetStartByDefault)),
		new BoolProvider<MusicPlayer>("Looping", SPH_BIND_GETTER(MusicPlayer::IsLooping), SPH_BIND_SETTER(MusicPlayer::SetLooping))
		});


	MusicPlayer::MusicPlayer(GameObject* parent, const std::string& name)
		: GameObject(parent, name)
	{
	}


	MusicPlayer::~MusicPlayer()
	{
		SoundPlayer::Stop(musicPath);
	}


	void MusicPlayer::CopyFrom(GameObject* other)
	{
		SPH_CHECKTYPE(other, MusicPlayer);

		GameObject::CopyFrom(other);
		MusicPlayer* trueOther = (MusicPlayer*)other;
		musicPath = trueOther->musicPath;
		loop = trueOther->loop;
		startByDefault = trueOther->startByDefault;
		wantToStart = startByDefault;
	}


	void MusicPlayer::Play()
	{
		wantToStart = true;
	}


	void MusicPlayer::Stop()
	{
		SoundPlayer::Stop(musicPath);
	}


	bool MusicPlayer::Load(FileLoader& fl)
	{
		GameObject::Load(fl);
		fl.Read(musicPath);
		fl.Read(loop);
		fl.Read(startByDefault);
		wantToStart = startByDefault;
		return fl.GetStatus();
	}


	bool MusicPlayer::Save(FileSaver& fs)
	{
		GameObject::Save(fs);
		fs.Write(musicPath);
		fs.Write(loop);
		fs.Write(startByDefault);
		return fs.GetStatus();
	}


	void MusicPlayer::Tick(float deltaTime)
	{
		if (wantToStart && !musicPath.empty()) {
			bool success = SoundPlayer::Play(musicPath, loop);
			wantToStart = false;
			SPH_ASSERT(success, "{0} is not a valid *.wav file", musicPath);
		}
	}

}
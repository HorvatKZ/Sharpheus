#include "pch.h"
#include "SoundEffects.hpp"
#include "Engine/Project.hpp"
#include "Engine/ResourceManager/AudioPlayer.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"


namespace Sharpheus {

	SPH_START_CLASSINFO(SoundEffects, "soundeffects.png")
		SPH_PROVIDE_STRINGLIST_NOCURR(SoundEffects, "Effects", AUDIO, GetNumOfAudios, GetAudioName, SetAudioFromPath, AddAudioFromPath, RemoveAudioByInd)
	SPH_END_CLASSINFO

	
	SoundEffects::EffectInstance::EffectInstance(SoLoud::handle handle, float length)
		: handle(handle), endTime(Project::GetTimeOfRun() + length)
	{
	}


	SoundEffects::SoundEffects(GameObject* parent, const std::string& name)
	: GameObject(parent, name)
	{
	}


	SoundEffects::~SoundEffects()
	{
		StopAll();
	}


	void SoundEffects::CopyFrom(GameObject* other)
	{
		SPH_CHECKTYPE(other, SoundEffects);

		GameObject::CopyFrom(other);
		SoundEffects* trueOther = (SoundEffects*)other;
		for (uint32 i = 0; i < trueOther->audios.size(); ++i) {
			audios.push_back(trueOther->audios[i]);
		}
	}


	bool SoundEffects::Load(FileLoader& fl)
	{
		GameObject::Load(fl);
		uint32 n;
		fl.Read(n);
		for (uint32 i = 0; i < n; ++i) {
			Audio* audio = nullptr;
			fl.Read(&audio);
			AddAudio(audio);
		}
		return fl.GetStatus();
	}


	bool SoundEffects::Save(FileSaver& fs)
	{
		GameObject::Save(fs);
		fs.Write((uint32_t)audios.size());
		for (uint32 i = 0; i < audios.size(); ++i) {
			fs.Write(audios[i]);
		}
		return fs.GetStatus();
	}


	void SoundEffects::Tick(float deltaTime)
	{
		float allTime = Project::GetTimeOfRun() + deltaTime;
		while (!activeHandles.empty() && activeHandles.top().endTime < allTime) {
			activeHandles.pop();
		}
	}


	bool SoundEffects::SetAudioFromPath(uint32 ind, const std::string& path)
	{
		return SetAudio(ind, ResourceManager::GetAudio(path));
	}


	bool SoundEffects::AddAudioFromPath(const std::string& path)
	{
		return AddAudio(ResourceManager::GetAudio(path));;
	}


	bool SoundEffects::RemoveAudio(Audio* audio)
	{
		return RemoveAudioByInd(GetIndOfAudio(audio->GetName()));
	}


	void SoundEffects::Play(uint32 ind, uint32 volume, float playSpeed)
	{
		if (ind >= audios.size()) {
			return;
		}

		SoLoud::handle handle = AudioPlayer::Play(audios[ind], false, volume, playSpeed);
		activeHandles.push(EffectInstance(handle, Project::GetTimeOfRun() + audios[ind]->GetLength() * playSpeed));
	}


	void SoundEffects::Play(const std::string& name, uint32 volume, float playSpeed)
	{
		Play(GetIndOfAudio(name), volume, playSpeed);
	}


	void SoundEffects::StopAll()
	{
		while (!activeHandles.empty()) {
			AudioPlayer::Stop(activeHandles.top().handle);
			activeHandles.pop();
		}
	}

}
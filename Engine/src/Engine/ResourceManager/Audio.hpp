#pragma once

#include "Resource.hpp"
#include <soloud_wav.h>


namespace Sharpheus {

	class SPH_EXPORT Audio : public Resource
	{
		friend class AudioPlayer;
	public:
		Audio(const std::string& path);
		~Audio();

		Audio(const Audio& anim) = delete;
		Audio(Audio&& anim) = delete;

		inline const std::string& GetName() const { return name; }
		inline float GetLength() const { return audio.getLength(); }

	private:
		std::string name;
		mutable SoLoud::Wav audio; // SoLoud is not const correct

		void InitName();
	};

}
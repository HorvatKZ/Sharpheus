#pragma once

#include "Resource.hpp"
#include <soloud_wav.h>


namespace Sharpheus {

	class SPH_EXPORT Audio : public Resource
	{
	public:
		Audio(const std::string& path);
		~Audio();

		inline const std::string& GetName() { return name; }
		inline float GetLength() { return audio.getLength(); }
		inline SoLoud::Wav& GetAudio() { return audio; }

	private:
		std::string name;
		SoLoud::Wav audio;

		void InitName();
	};

}
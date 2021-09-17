#include "pch.h"
#include "Audio.hpp"


namespace Sharpheus {
	
	Audio::Audio(const std::string& path)
		: Resource(path)
	{
		audio.load(fullPath.c_str());
		valid = (audio.getLength() != 0.0);

		InitName();
	}


	Audio::~Audio()
	{
		audio.stop();
	}


	void Audio::InitName()
	{
		uint32 lastSlash = path.find_last_of('/');
		uint32 lastBackSlash = path.find_last_of('\\');
		uint32 length = path.length();
		if (lastSlash >= length) {
			lastSlash = 0;
		}
		if (lastBackSlash >= length) {
			lastBackSlash = 0;
		}
		uint32 start = (lastSlash > lastBackSlash) ? (lastSlash + 1) : (lastBackSlash + 1);
		uint32 lastDot = path.find_last_of('.');
		if (lastDot >= length || lastDot < start) {
			lastDot = start;
		}
		uint32 nameLength = lastDot - start;
		name = path.substr(start, nameLength);
	}

}
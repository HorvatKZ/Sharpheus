#pragma once

#include "pch.h"
#include <cstdio>


namespace Sharpheus {

	class SPH_EXPORT FileSaver
	{
	public:
		FileSaver(const std::string& path);
		virtual ~FileSaver();

		inline bool GetStatus() { return status; }

		bool Write(uint8 data);
		bool Write(uint32 data);
		bool Write(int32 data);
		bool Write(size_t data);
		bool Write(bool data);
		bool Write(float data);
		bool Write(const std::string& data);
		bool Write(const Point& data);
		bool Write(const Color& data);
		bool Write(const Transform& data);
		bool Write(const class Image* data);
		bool Write(const class Font* data);
		bool Write(const class Animation* data);
		bool Write(const class TileSet* data);
		bool Write(const class Audio* data);

		bool WriteEnd();

	private:
		FILE* file;
		bool status = true;
	};

}
#pragma once

#include "pch.h"
#include <cstdio>


namespace Sharpheus {

	class FileSaver
	{
	public:
		FileSaver(const std::string& path);
		virtual ~FileSaver();

		inline bool GetStatus() { return status; }

		bool Write(uint8_t data);
		bool Write(uint32_t data);
		bool Write(int32_t data);
		bool Write(size_t data);
		bool Write(bool data);
		bool Write(float data);
		bool Write(const std::string& data);
		bool Write(const Point& data);
		bool Write(const Color& data);
		bool Write(const Transform& data);
		bool Write(class Image* data);
		bool Write(class Font* data);
		bool Write(class Animation* data);
		bool Write(class TileSet* data);

		bool WriteEnd();

	private:
		FILE* file;
		bool status = true;
	};

}
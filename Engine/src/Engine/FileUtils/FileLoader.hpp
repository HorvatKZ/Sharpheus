#pragma once

#include "pch.h"
#include <cstdio>


namespace Sharpheus {

	class SPH_EXPORT FileLoader
	{
	public:
		FileLoader(const std::string& path);
		virtual ~FileLoader();

		inline bool GetStatus() { return status; }

		bool Read(uint8& data);
		bool Read(uint32& data);
		bool Read(int32& data);
		bool Read(size_t& data);
		bool Read(bool& data);
		bool Read(float& data);
		bool Read(std::string& data);
		bool Read(Point& data);
		bool Read(Color& data);
		bool Read(Transform& data);
		bool Read(const class Image** data);
		bool Read(const class Font** data);
		bool Read(const class Animation** data);
		bool Read(const class TileSet** data);
		bool Read(const class Audio** data);

		bool ReadLine();

		bool TryReadingEnd();

	private:
		FILE* file;
		bool status = true;
		bool newLineRead = false;
	};

}
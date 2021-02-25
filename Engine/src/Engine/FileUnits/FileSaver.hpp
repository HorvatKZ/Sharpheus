#pragma once

#include "pch.h"


namespace Sharpheus {

	class FileSaver
	{
	public:
		FileSaver(const std::string& path);
		virtual ~FileSaver();

		bool GetStatus();

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

		bool WriteEnd();

	private:
		std::ofstream file;
	};

}
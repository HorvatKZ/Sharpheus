#pragma once

#include "pch.h"


namespace Sharpheus {

	class FileLoader
	{
	public:
		FileLoader(const std::string& path);
		virtual ~FileLoader();

		bool GetStatus();

		bool Read(uint8_t& data);
		bool Read(uint32_t& data);
		bool Read(int32_t& data);
		bool Read(size_t& data);
		bool Read(bool& data);
		bool Read(float& data);
		bool Read(std::string& data);
		bool Read(Point& data);
		bool Read(Color& data);
		bool Read(Transform& data);
		bool Read(class Image** data);

		bool TryReadingEnd();

	private:
		std::ifstream file;
	};

}
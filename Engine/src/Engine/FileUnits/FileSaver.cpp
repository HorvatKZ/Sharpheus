#include "pch.h"
#include "FileSaver.hpp"
#include "Engine/ResourceManager/Font.hpp"


namespace Sharpheus {

	FileSaver::FileSaver(const std::string& path) : file(path.c_str(), std::ios::out | std::ios::binary)
	{
		SPH_ASSERT(file, "Cannot create/open file \"{0}\" to save", path);
	}


	FileSaver::~FileSaver()
	{
		file.close();
	}


	bool FileSaver::GetStatus()
	{
		return !file.fail();
	}


	bool FileSaver::Write(uint8_t data)
	{
		if (file) {
			file.write((const char*)&data, sizeof(data));
		}
		return GetStatus();
	}


	bool FileSaver::Write(uint32_t data)
	{
		if (file) {
			file.write((const char*)&data, sizeof(data));
		}
		return GetStatus();
	}


	bool FileSaver::Write(int32_t data)
	{
		if (file) {
			file.write((const char*)&data, sizeof(data));
		}
		return GetStatus();
	}


	bool FileSaver::Write(size_t data)
	{
		if (file) {
			file.write((const char*)&data, sizeof(data));
		}
		return GetStatus();
	}


	bool FileSaver::Write(bool data)
	{
		if (file) {
			char _data = data ? 1 : 0;
			file.write((const char*)&_data, sizeof(_data));
		}
		return GetStatus();
	}


	bool FileSaver::Write(float data)
	{
		if (file) {
			file.write((const char*)&data, sizeof(data));
		}
		return GetStatus();
	}


	bool FileSaver::Write(const std::string& data)
	{
		if (file) {
			uint32_t len = data.length();
			file.write((const char*)&len, sizeof(len));
			file.write((const char*)data.c_str(), sizeof(char) * (len + 1));
		}
		return GetStatus();
	}


	bool FileSaver::Write(const Point& data)
	{
		if (file) {
			file.write((const char*)&data, sizeof(data));
		}
		return GetStatus();
	}


	bool FileSaver::Write(const Color& data)
	{
		if (file) {
			file.write((const char*)&data, sizeof(data));
		}
		return GetStatus();
	}


	bool FileSaver::Write(const Transform& data)
	{
		if (file) {
			file.write((const char*)&data, sizeof(data));
		}
		return GetStatus();
	}


	bool FileSaver::Write(Image* data)
	{
		if (data == nullptr) {
			Write("nullptr");
			return GetStatus();
		}

		Write(data->GetPath());
		Write(data->IsFiltered());
		return GetStatus();
	}


	bool FileSaver::Write(Font* data)
	{
		if (data == nullptr) {
			Write("nullptr");
			return GetStatus();
		}

		Write(data->GetPath());
		Write(data->GetImage());
		return GetStatus();
	}


	bool FileSaver::WriteEnd()
	{
		if (file) {
			char data = '\n';
			file.write((const char*)&data, sizeof(data));
		}
		return GetStatus();
	}

}
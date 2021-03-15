#include "pch.h"
#include "FileLoader.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"


namespace Sharpheus {

	FileLoader::FileLoader(const std::string& path) : file(path.c_str(), std::ios::in | std::ios::binary)
	{
		SPH_ASSERT(file, "Cannot create/open file \"{0}\" to load", path);
	}


	FileLoader::~FileLoader()
	{
		file.close();
	}


	bool FileLoader::GetStatus()
	{
		return !file.fail();
	}


	bool FileLoader::Read(uint8_t& data)
	{
		if (file) {
			file.read((char*)&data, sizeof(data));
		}
		return GetStatus();
	}


	bool FileLoader::Read(uint32_t& data)
	{
		if (file) {
			file.read((char*)&data, sizeof(data));
		}
		return GetStatus();
	}


	bool FileLoader::Read(int32_t& data)
	{
		if (file) {
			file.read((char*)&data, sizeof(data));
		}
		return GetStatus();
	}


	bool FileLoader::Read(size_t& data)
	{
		if (file) {
			file.read((char*)&data, sizeof(data));
		}
		return GetStatus();
	}


	bool FileLoader::Read(bool& data)
	{
		if (file) {
			char _data;
			file.read((char*)&_data, sizeof(_data));
			data = _data != 0;
		}
		return GetStatus();
	}


	bool FileLoader::Read(float& data)
	{
		if (file) {
			file.read((char*)&data, sizeof(data));
		}
		return GetStatus();
	}


	bool FileLoader::Read(std::string& data)
	{
		if (file) {
			uint32_t len = 0;
			file.read((char*)&len, sizeof(len));
			char* str = new char[len + 1];
			file.read(str, sizeof(char) * (len + 1));
			data = str;
			delete str;
		}
		return GetStatus();
	}


	bool FileLoader::Read(Point& data)
	{
		if (file) {
			file.read((char*)&data, sizeof(data));
		}
		return GetStatus();
	}


	bool FileLoader::Read(Color& data)
	{
		if (file) {
			file.read((char*)&data, sizeof(data));
		}
		return GetStatus();
	}


	bool FileLoader::Read(Transform& data)
	{
		if (file) {
			file.read((char*)&data, sizeof(data));
		}
		return GetStatus();
	}


	bool FileLoader::Read(Image** data)
	{
		std::string path;
		bool filtered;
		Read(path);
		if (path == "nullptr") {
			*data = nullptr;
			return GetStatus();
		}
		Read(filtered);
		*data = ResourceManager::GetImage(path, filtered);
		return GetStatus();
	}


	bool FileLoader::TryReadingEnd()
	{
		char data;
		if (file) {
			file.read((char*)&data, sizeof(data));
		}
		return GetStatus() && data == '\n';
	}
}
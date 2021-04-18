#include "pch.h"
#include "FileLoader.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"


namespace Sharpheus {

	FileLoader::FileLoader(const std::string& path)
	{
		file = fopen(path.c_str(), "rb");
		SPH_ASSERT(file != NULL, "Cannot create/open file \"{0}\" to load", path);
	}


	FileLoader::~FileLoader()
	{
		fclose(file);
	}


	bool FileLoader::Read(uint8_t& data)
	{
		if (file != NULL) {
			size_t result = fread(&data, sizeof(data), 1, file);
			status &= result == 1;
		}
		return GetStatus();
	}


	bool FileLoader::Read(uint32_t& data)
	{
		if (file != NULL) {
			size_t result = fread(&data, sizeof(data), 1, file);
			status &= result == 1;
		}
		return GetStatus();
	}


	bool FileLoader::Read(int32_t& data)
	{
		if (file != NULL) {
			size_t result = fread(&data, sizeof(data), 1, file);
			status &= result == 1;
		}
		return GetStatus();
	}


	bool FileLoader::Read(size_t& data)
	{
		if (file != NULL) {
			size_t result = fread(&data, sizeof(data), 1, file);
			status &= result == 1;
		}
		return GetStatus();
	}


	bool FileLoader::Read(bool& data)
	{
		if (file != NULL) {
			char _data;
			size_t result = fread(&_data, sizeof(_data), 1, file);
			status &= result == 1;
			data = _data != 0;
		}
		return GetStatus();
	}


	bool FileLoader::Read(float& data)
	{
		if (file != NULL) {
			size_t result = fread(&data, sizeof(data), 1, file);
			status &= result == 1;
		}
		return GetStatus();
	}


	bool FileLoader::Read(std::string& data)
	{
		if (file) {
			uint32_t len = 0;
			size_t result = fread(&len, sizeof(len), 1, file);
			status &= result == 1;
			if (status) {
				char* str = new char[len + 1];
				result = fread(str, sizeof(char) * (len + 1), 1, file);
				status &= result == 1;
				data = str;
				delete str;
			}
		}
		return GetStatus();
	}


	bool FileLoader::Read(Point& data)
	{
		if (file != NULL) {
			size_t result = fread(&data, sizeof(data), 1, file);
			status &= result == 1;
		}
		return GetStatus();
	}


	bool FileLoader::Read(Color& data)
	{
		if (file != NULL) {
			size_t result = fread(&data, sizeof(data), 1, file);
			status &= result == 1;
		}
		return GetStatus();
	}


	bool FileLoader::Read(Transform& data)
	{
		if (file != NULL) {
			size_t result = fread(&data, sizeof(data), 1, file);
			status &= result == 1;
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


	bool FileLoader::Read(Font** data)
	{
		std::string path;
		Image* img;
		Read(path);
		if (path == "nullptr") {
			*data = nullptr;
			return GetStatus();
		}
		Read(&img);
		*data = ResourceManager::GetFont(path, img);
		return GetStatus();
	}


	bool FileLoader::Read(Animation** data)
	{
		std::string path;
		Read(path);
		if (path == "nullptr") {
			*data = nullptr;
			return GetStatus();
		}

		*data = ResourceManager::GetAnimation(path);
		return GetStatus();
	}


	bool FileLoader::TryReadingEnd()
	{
		char data;
		if (file != NULL) {
			size_t result = fread(&data, sizeof(data), 1, file);
			status &= result == 1;
		}
		return GetStatus() && data == '\n';
	}
}
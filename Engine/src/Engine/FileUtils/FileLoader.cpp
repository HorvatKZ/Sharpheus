#include "pch.h"
#include "FileLoader.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"


namespace Sharpheus {

	FileLoader::FileLoader(const std::string& path)
	{
		file = fopen(path.c_str(), "rb");
		if (file == NULL) {
			status = false;
			SPH_ERROR("Cannot open file \"{0}\" to load", path);
		}
	}


	FileLoader::~FileLoader()
	{
		if (file != NULL) {
			fclose(file);
		}
	}


	bool FileLoader::Read(uint8& data)
	{
		if (file != NULL) {
			size_t result = fread(&data, sizeof(data), 1, file);
			status &= result == 1;
		}
		return GetStatus();
	}


	bool FileLoader::Read(uint32& data)
	{
		if (file != NULL) {
			size_t result = fread(&data, sizeof(data), 1, file);
			status &= result == 1;
		}
		return GetStatus();
	}


	bool FileLoader::Read(int32& data)
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
			uint32 len = 0;
			size_t result = fread(&len, sizeof(len), 1, file);
			status &= result == 1;
			if (status) {
				char* str = new char[len + 1];
				result = fread(str, sizeof(char) * (len + 1), 1, file);
				status &= (result == 1 && str[len] == '\0');
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


	bool FileLoader::Read(const Image** data)
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


	bool FileLoader::Read(const Font** data)
	{
		std::string path;
		const Image* img;
		Read(path);
		if (path == "nullptr") {
			*data = nullptr;
			return GetStatus();
		}
		Read(&img);
		*data = ResourceManager::GetFont(path, img);
		return GetStatus();
	}


	bool FileLoader::Read(const Animation** data)
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


	bool FileLoader::Read(const TileSet** data)
	{
		std::string path;
		Read(path);
		if (path == "nullptr") {
			*data = nullptr;
			return GetStatus();
		}

		*data = ResourceManager::GetTileSet(path);
		return GetStatus();
	}


	bool FileLoader::Read(const Audio** data)
	{
		std::string path;
		Read(path);
		if (path == "nullptr") {
			*data = nullptr;
			return GetStatus();
		}

		*data = ResourceManager::GetAudio(path);
		return GetStatus();
	}


	bool FileLoader::ReadLine()
	{
		char c;
		size_t result = fread(&c, sizeof(c), 1, file);
		status &= result == 1;
		while (status && c != '\n') {
			size_t result = fread(&c, sizeof(c), 1, file);
			status &= result == 1;
		}

		newLineRead = (c == '\n');
		return GetStatus();
	}


	bool FileLoader::TryReadingEnd()
	{
		if (newLineRead) {
			newLineRead = false;
			return GetStatus();
		}

		char data;
		if (file != NULL) {
			size_t result = fread(&data, sizeof(data), 1, file);
			status &= result == 1;
		}
		return GetStatus() && data == '\n';
	}
}
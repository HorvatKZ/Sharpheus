#include "pch.h"
#include "FileSaver.hpp"
#include "Engine/ResourceManager/Font.hpp"
#include "Engine/ResourceManager/Animation.hpp"
#include "Engine/ResourceManager/TileSet.hpp"
#include "Engine/ResourceManager/Audio.hpp"


namespace Sharpheus {

	FileSaver::FileSaver(const std::string& path)
	{
		file = fopen(path.c_str(), "wb");
		if (file == NULL) {
			status = false;
			SPH_ERROR("Cannot create/open file \"{0}\" to load", path);
		}
	}


	FileSaver::~FileSaver()
	{
		if (file != NULL) {
			fclose(file);
		}
	}


	bool FileSaver::Write(uint8 data)
	{
		if (file != NULL) {
			size_t result = fwrite((const char*)&data, sizeof(data), 1, file);
			status &= result == 1;
		}
		return GetStatus();
	}


	bool FileSaver::Write(uint32 data)
	{
		if (file != NULL) {
			size_t result = fwrite((const char*)&data, sizeof(data), 1, file);
			status &= result == 1;
		}
		return GetStatus();
	}


	bool FileSaver::Write(int32 data)
	{
		if (file != NULL) {
			size_t result = fwrite((const char*)&data, sizeof(data), 1, file);
			status &= result == 1;
		}
		return GetStatus();
	}


	bool FileSaver::Write(size_t data)
	{
		if(file != NULL) {
			size_t result = fwrite((const char*)&data, sizeof(data), 1, file);
			status &= result == 1;
		}
		return GetStatus();
	}


	bool FileSaver::Write(bool data)
	{
		if (file != NULL) {
			char _data = data ? 1 : 0;
			size_t result = fwrite((const char*)&_data, sizeof(_data), 1, file);
			status &= result == 1;
		}
		return GetStatus();
	}


	bool FileSaver::Write(float data)
	{
		if (file != NULL) {
			size_t result = fwrite((const char*)&data, sizeof(data), 1, file);
			status &= result == 1;
		}
		return GetStatus();
	}


	bool FileSaver::Write(const std::string& data)
	{
		if (file != NULL) {
			uint32 len = data.length();
			size_t result = fwrite((const char*)&len, sizeof(len), 1, file);
			status &= result == 1;
			if (status) {
				result = fwrite((const char*)data.c_str(), sizeof(char) * (len + 1), 1, file);
				status &= result == 1;
			}
		}
		return GetStatus();
	}


	bool FileSaver::Write(const Point& data)
	{
		if (file != NULL) {
			size_t result = fwrite((const char*)&data, sizeof(data), 1, file);
			status &= result == 1;
		}
		return GetStatus();
	}


	bool FileSaver::Write(const Color& data)
	{
		if (file != NULL) {
			size_t result = fwrite((const char*)&data, sizeof(data), 1, file);
			status &= result == 1;
		}
		return GetStatus();
	}


	bool FileSaver::Write(const Transform& data)
	{
		if (file != NULL) {
			size_t result = fwrite((const char*)&data, sizeof(data), 1, file);
			status &= result == 1;
		}
		return GetStatus();
	}


	bool FileSaver::Write(Image* data)
	{
		if (data == nullptr) {
			Write(std::string("nullptr"));
			return GetStatus();
		}

		Write(data->GetPath());
		Write(data->IsFiltered());
		return GetStatus();
	}


	bool FileSaver::Write(Font* data)
	{
		if (data == nullptr) {
			Write(std::string("nullptr"));
			return GetStatus();
		}

		Write(data->GetPath());
		Write(data->GetImage());
		return GetStatus();
	}


	bool FileSaver::Write(Animation* data)
	{
		if (data == nullptr) {
			Write(std::string("nullptr"));
			return GetStatus();
		}

		Write(data->GetPath());
		return GetStatus();
	}


	bool FileSaver::Write(TileSet* data)
	{
		if (data == nullptr) {
			Write(std::string("nullptr"));
			return GetStatus();
		}

		Write(data->GetPath());
		return GetStatus();
	}


	bool FileSaver::Write(Audio* data)
	{
		if (data == nullptr) {
			Write(std::string("nullptr"));
			return GetStatus();
		}

		Write(data->GetPath());
		return GetStatus();
	}


	bool FileSaver::WriteEnd()
	{
		if (file != NULL) {
			char data = '\n';
			size_t result = fwrite((const char*)&data, sizeof(data), 1, file);
			status &= result == 1;
		}
		return GetStatus();
	}

}
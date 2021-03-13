#include "pch.h"
#include "OSPaths.hpp"
#include <locale>
#include <codecvt>


namespace Sharpheus {

	std::string OSPaths::Get(Folder folder)
	{
		switch (folder) {
			case Folder::APPDATA_ROAMING:
				return GetWin(FOLDERID_RoamingAppData);
			default:
				return "";
		}
	}


	std::string OSPaths::GetWin(REFKNOWNFOLDERID rfid)
	{
		PWSTR path = NULL;
		HRESULT hr = SHGetKnownFolderPath(rfid, 0, NULL, &path);

		std::string result;

		if (SUCCEEDED(hr)) {
			std::wstring wresult(path);
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			result = converter.to_bytes(wresult);
		}

		CoTaskMemFree(path);

		return result;
	}

}
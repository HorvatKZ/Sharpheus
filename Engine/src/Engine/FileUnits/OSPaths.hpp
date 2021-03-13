#pragma once

#include <string>
#include "shlobj.h"


namespace Sharpheus {

	class OSPaths
	{
	public:
		enum class Folder {
			APPDATA_ROAMING
		};

		static std::string Get(Folder folder);
	
	private:
		static std::string GetWin(REFKNOWNFOLDERID rfid);
	};

}
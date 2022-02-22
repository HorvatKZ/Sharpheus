#pragma once

#include <string>
#include "shlobj.h"


namespace Sharpheus {

	class SPH_EXPORT OSPaths
	{
	public:
		enum class Folder {
			APPDATA_ROAMING,
			EXEC_FOLDER
		};

		static std::string Get(Folder folder);
	
	private:
		static std::string GetWin(REFKNOWNFOLDERID rfid);
		static std::string GetExecFolderWin();
	};

}
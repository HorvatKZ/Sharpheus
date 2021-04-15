#pragma once

#include <string>


namespace Sharpheus {

	std::string wxStr2StdStr(const class wxString& str);


	class UI
	{
	public:
		static uint32_t border;
		static uint32_t scrollSpeed;
		static wxSize buttonSize;
		static wxFont titleFont;
	};

}
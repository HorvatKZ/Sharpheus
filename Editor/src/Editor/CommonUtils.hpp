#pragma once

#include <string>


namespace Sharpheus {

	std::string wxStr2StdStr(const class wxString& str);


	class UI
	{
	public:
		static uint32_t UI::border;
		static wxSize UI::buttonSize;
		static wxFont titleFont;
	};

}
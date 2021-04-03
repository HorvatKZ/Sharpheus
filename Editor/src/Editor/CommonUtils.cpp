#include "editor_pch.h"
#include "CommonUtils.hpp"


namespace Sharpheus {

	std::string wxStr2StdStr(const wxString& str) {
		uint32_t len = str.length();
		std::string result(len, ' ');
		for (uint32_t i = 0; i < len; ++i) {
			result[i] = str[i];
		}
		return result;
	}


	uint32_t UI::border = 5;
	wxSize UI::buttonSize(90, 26);
	wxFont UI::titleFont(10, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
}
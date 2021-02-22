#include "editor_pch.h"
#include "CommonUtils.hpp"


namespace Sharpheus {

	std::string wxStr2StdStr(const wxString& str) {
		uint32_t len = str.length();
		char* c_str = new char[len]();
		for (uint32_t i = 0; i < len; ++i) {
			c_str[i] = str[i];
		}
		std::string result(c_str, len);
		delete[] c_str;
		return result;
	}
}
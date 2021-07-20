#include "editor_pch.h"
#include "StringUtils.hpp"


namespace Sharpheus {

	std::string wxStr2StdStr(const wxString& str) {
		uint32 len = str.length();
		std::string result(len, ' ');
		for (uint32 i = 0; i < len; ++i) {
			result[i] = str[i];
		}
		return result;
	}

}
#pragma once
#include <string>

namespace Sharpheus {

	struct EngineVersion {
		uint32 num;
		std::string str;

		EngineVersion() : num(0), str("unknown") {}

		EngineVersion(uint32 num, const std::string& str)
			: num(num), str(str) {}

		static EngineVersion latest;
	};

}


#include "pch.h"
#include "EngineVersion.hpp"


namespace Sharpheus {

	const std::unordered_map<std::string, uint32> EngineVersion::versions({
		{"unknown", 0x00},
		{"alpha1", 0x01}
	});

	const EngineVersion EngineVersion::latest("alpha1");
	EngineVersion EngineVersion::current = EngineVersion::latest;


	EngineVersion::EngineVersion() : vnum(0), vname("unknown")
	{
	}


	EngineVersion::EngineVersion(const std::string& vname)
	{
		uint32 vnum = GetVNumOf(vname);
		if (vnum == 0) {
			this->vname = "unknown";
		} else {
			this->vname = vname;
		}
		this->vnum = vnum;
	}

}
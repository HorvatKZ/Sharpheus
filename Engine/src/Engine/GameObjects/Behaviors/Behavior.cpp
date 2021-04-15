#include "pch.h"
#include "Behavior.hpp"


namespace Sharpheus {

	ClassInfo Behavior::classInfo("Behavior", "behavior.png", {});


	bool Behavior::Save(FileSaver& fs)
	{
		fs.Write((uint8_t)GetType());
		fs.Write(name);
		fs.Write(GetSubType());
		fs.Write((uint32_t)children.size());
		fs.Write(trafo);
		fs.Write(isVisible);
		return fs.GetStatus();
	}

}
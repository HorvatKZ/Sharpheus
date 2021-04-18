#include "pch.h"
#include "Behavior.hpp"
#include "Engine/Level.hpp"


namespace Sharpheus {

	ClassInfo Behavior::classInfo("Behavior", "behavior.png", {});


	Behavior::Behavior(Behavior* other) : GameObject(std::move(*other))
	{
		delete other;
		other = nullptr;
		level->RequestInit(this);
	}

	void Behavior::SetLevel(Level* level)
	{
		GameObject::SetLevel(level);
		level->RequestInit(this);
	}


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
#include "pch.h"
#include "Behavior.hpp"
#include "Engine/Level.hpp"


namespace Sharpheus {

	SPH_START_SPEC_CLASSINFO(Behavior, "CppBehavior", "behaviorcpp.png")
	SPH_END_CLASSINFO


	Behavior::Behavior(Behavior* other) : GameObject(std::move(*other))
	{
		delete other;
		other = nullptr;
		level->RequestInit(this);
	}


	void Behavior::CopyFrom(GameObject* other)
	{
		SPH_CHECKMASK(other, Behavior);
		if (((Behavior*)other)->GetSubType() != GetSubType()) {
			SPH_ERROR("\"{0}\" tries to copy from different type \"{1}\"", name, other->GetName());
			return;
		}
		CopyContent(other);
	}


	void Behavior::SetLevel(Level* level)
	{
		GameObject::SetLevel(level);
		level->RequestInit(this);
	}


	bool Behavior::Save(FileSaver& fs)
	{
		fs.Write((uint8)GetType());
		fs.Write(name);
		fs.Write(GetSubType());
		fs.Write((uint32)children.size());
		fs.Write(trafo);
		fs.Write(isVisible);
		return fs.GetStatus();
	}


	void Behavior::CopyContent(GameObject* other)
	{
	}

}
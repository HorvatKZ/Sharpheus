#include "pch.h"
#include "HPPackControl.hpp"
#include "Engine/Level.hpp"


using namespace Sharpheus;


uint32_t HPPackControl::classVersion = 1;

ClassInfo HPPackControl::classInfo("HPPackControl", "behavior.png", {
	new UIntProvider<HPPackControl>("Heal", SPH_BIND_GETTER(HPPackControl::GetHeal), SPH_BIND_SETTER(HPPackControl::SetHeal))
});


HPPackControl::HPPackControl(Sharpheus::Behavior* other)
	: Sharpheus::Behavior(other), ColliderListener(GetID())
{
}


HPPackControl::HPPackControl(Sharpheus::GameObject* parent, const std::string& name)
	: Sharpheus::Behavior(parent, name), ColliderListener(GetID())
{
}


HPPackControl::~HPPackControl()
{
}


bool HPPackControl::Load(FileLoader& fl)
{
	Sharpheus::Behavior::Load(fl);
	SPH_CHECK_CLASSVERSION(fl, classVersion);
	fl.Read(heal);
	return fl.GetStatus();
}


bool HPPackControl::Save(FileSaver& fs)
{
	Sharpheus::Behavior::Save(fs);
	fs.Write(classVersion);
	fs.Write(heal);
	return fs.GetStatus();
}


void HPPackControl::Tick(float deltaTime)
{
	// Called once per tick
}


void HPPackControl::CopyContent(GameObject* other)
{
	Behavior::CopyContent(other);

	HPPackControl* trueOther = (HPPackControl*)other;
	heal = trueOther->heal;
}


void HPPackControl::OnTriggerEnter(const OnEnterEvent& e)
{
	if (player && e.obj->GetID() == player->GetID()) {
		player->Heal(heal);
		level->Delete(parent);
	}
}


void HPPackControl::Init()
{
	player = (PlayerController*)level->GetGameObject("PlayerController");

	SubscribeOnTriggerEnter((Collider*)parent->GetFirstChildOfMask(TypeMasks::Collider), SPH_BIND(HPPackControl::OnTriggerEnter));
}


bool HPPackControl::IsCompatibleWithParent(GameObject* parent)
{
	return parent != nullptr;
}


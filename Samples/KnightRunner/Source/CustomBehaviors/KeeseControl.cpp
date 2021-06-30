#include "pch.h"
#include "KeeseControl.hpp"


using namespace Sharpheus;


uint32_t KeeseControl::classVersion = 1;

ClassInfo KeeseControl::classInfo("KeeseControl", "behavior.png", {
	new UIntProvider<KeeseControl>("Max HP", SPH_BIND_GETTER(KeeseControl::GetMaxHP), SPH_BIND_SETTER(KeeseControl::SetMaxHP)),
	new UIntProvider<KeeseControl>("Damage", SPH_BIND_GETTER(KeeseControl::GetDMG), SPH_BIND_SETTER(KeeseControl::SetDMG)),
	new UFloatProvider<KeeseControl>("Attack speed", SPH_BIND_GETTER(KeeseControl::GetAttackSpeed), SPH_BIND_SETTER(KeeseControl::SetAttackSpeed)),
	new UFloatProvider<KeeseControl>("Speed", SPH_BIND_GETTER(KeeseControl::GetSpeed), SPH_BIND_SETTER(KeeseControl::SetSpeed)),
	new UFloatProvider<KeeseControl>("Sense", SPH_BIND_GETTER(KeeseControl::GetSense), SPH_BIND_SETTER(KeeseControl::SetSense))
	});


KeeseControl::KeeseControl(Sharpheus::Behavior* other)
	: EnemyControl(other)
{
}


KeeseControl::KeeseControl(Sharpheus::GameObject* parent, const std::string& name)
	: EnemyControl(parent, name)
{
}


KeeseControl::~KeeseControl()
{
}


bool KeeseControl::Load(FileLoader& fl)
{
	EnemyControl::Load(fl);
	SPH_CHECK_CLASSVERSION(fl, classVersion);
	fl.Read(speed);
	fl.Read(sense);
	return fl.GetStatus();
}


bool KeeseControl::Save(FileSaver& fs)
{
	EnemyControl::Save(fs);
	fs.Write(classVersion);
	fs.Write(speed);
	fs.Write(sense);
	return fs.GetStatus();
}


void KeeseControl::Tick(float deltaTime)
{
	EnemyControl::Tick(deltaTime);

	if (player) {
		Transform trafo = parent->GetTrafo();
		Point playerPos = player->GetWorldTrafo().pos;
		if (trafo.pos.DistanceSquared(playerPos) < sense * sense) {
			Point dir = (playerPos - trafo.pos).Normalize();
			trafo.pos += dir * speed * deltaTime;
			parent->SetTrafo(trafo);
		}
	}
}


void KeeseControl::CopyContent(GameObject* other)
{
	EnemyControl::CopyContent(other);

	KeeseControl* trueOther = (KeeseControl*)other;
	speed = trueOther->speed;
	sense = trueOther->sense;
}


void KeeseControl::Init()
{
	EnemyControl::Init();
}


bool KeeseControl::IsCompatibleWithParent(GameObject* parent)
{
	return EnemyControl::IsCompatibleWithParent(parent);
}


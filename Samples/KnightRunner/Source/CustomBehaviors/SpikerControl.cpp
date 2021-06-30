#include "pch.h"
#include "SpikerControl.hpp"


using namespace Sharpheus;


uint32_t SpikerControl::classVersion = 1;

ClassInfo SpikerControl::classInfo("SpikerControl", "behavior.png", {
	new UIntProvider<SpikerControl>("Max HP", SPH_BIND_GETTER(SpikerControl::GetMaxHP), SPH_BIND_SETTER(SpikerControl::SetMaxHP)),
	new UIntProvider<SpikerControl>("Damage", SPH_BIND_GETTER(SpikerControl::GetDMG), SPH_BIND_SETTER(SpikerControl::SetDMG)),
	new UFloatProvider<SpikerControl>("Attack speed", SPH_BIND_GETTER(SpikerControl::GetAttackSpeed), SPH_BIND_SETTER(SpikerControl::SetAttackSpeed)),
	new UFloatProvider<SpikerControl>("Speed", SPH_BIND_GETTER(SpikerControl::GetSpeed), SPH_BIND_SETTER(SpikerControl::SetSpeed)),
	new UFloatProvider<SpikerControl>("Range", SPH_BIND_GETTER(SpikerControl::GetRange), SPH_BIND_SETTER(SpikerControl::SetRange))
});


SpikerControl::SpikerControl(Sharpheus::Behavior* other)
	: EnemyControl(other)
{
}


SpikerControl::SpikerControl(Sharpheus::GameObject* parent, const std::string& name)
	: EnemyControl(parent, name)
{
}


SpikerControl::~SpikerControl()
{
}


bool SpikerControl::Load(FileLoader& fl)
{
	EnemyControl::Load(fl);
	SPH_CHECK_CLASSVERSION(fl, classVersion);
	fl.Read(speed);
	fl.Read(range);
	return fl.GetStatus();
}


bool SpikerControl::Save(FileSaver& fs)
{
	EnemyControl::Save(fs);
	fs.Write(classVersion);
	fs.Write(speed);
	fs.Write(range);
	return fs.GetStatus();
}


void SpikerControl::Tick(float deltaTime)
{
	EnemyControl::Tick(deltaTime);

	Transform trafo = parent->GetTrafo();
	if (movesToRight) {
		float movement = speed * deltaTime;
		currPlaceInRange += movement;
		trafo.pos.x += movement;
		if (currPlaceInRange >= range) {
			movesToRight = false;
		}
	} else {
		float movement = speed * deltaTime;
		currPlaceInRange -= movement;
		trafo.pos.x -= movement;
		if (currPlaceInRange <= 0.f) {
			movesToRight = true;
		}
	}
	parent->SetTrafo(trafo);
}


void SpikerControl::CopyContent(GameObject* other)
{
	EnemyControl::CopyContent(other);

	SpikerControl* trueOther = (SpikerControl*)other;
	speed = trueOther->speed;
	range = trueOther->range;
}


void SpikerControl::Init()
{
	EnemyControl::Init();
}


bool SpikerControl::IsCompatibleWithParent(GameObject* parent)
{
	return EnemyControl::IsCompatibleWithParent(parent);
}


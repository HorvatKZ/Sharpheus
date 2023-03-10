#include "pch.h"
#include "PipeCtrl.hpp"
#include "Engine/Level.hpp"
#include "GameState.hpp"


using namespace Sharpheus;


const uint32_t PipeCtrl::classVersion = 1;

SPH_START_CLASSINFO(PipeCtrl, "behaviorcpp.png")
	SPH_PROVIDE_UFLOAT(PipeCtrl, "SpawnDist", GetSpawnDist, SetSpawnDist)
	SPH_PROVIDE_UFLOAT(PipeCtrl, "X distance", GetXDistance, SetXDistance)
	SPH_PROVIDE_UFLOAT(PipeCtrl, "X distribution", GetXDistribution, SetXDistribution)
	SPH_PROVIDE_UFLOAT(PipeCtrl, "Y distribution", GetYDistribution, SetYDistribution)
	SPH_PROVIDE_UFLOAT(PipeCtrl, "Y diff", GetYDiff, SetYDiff)
SPH_END_CLASSINFO


PipeCtrl::PipeCtrl(Sharpheus::Behavior* other)
	: Sharpheus::Behavior(other)
{
}


PipeCtrl::PipeCtrl(Sharpheus::GameObject* parent, const std::string& name)
	: Sharpheus::Behavior(parent, name)
{
}


PipeCtrl::~PipeCtrl()
{
}


bool PipeCtrl::Load(FileLoader& fl)
{
	Sharpheus::Behavior::Load(fl);
	SPH_CHECK_CLASSVERSION(fl, classVersion);
	fl.Read(spawnDist);
	fl.Read(xDistance);
	fl.Read(xDistribution);
	fl.Read(yDistribution);
	fl.Read(yDiff);
	return fl.GetStatus();
}


bool PipeCtrl::Save(FileSaver& fs)
{
	Sharpheus::Behavior::Save(fs);
	fs.Write(classVersion);
	fs.Write(spawnDist);
	fs.Write(xDistance);
	fs.Write(xDistribution);
	fs.Write(yDistribution);
	fs.Write(yDiff);
	return fs.GetStatus();
}


void PipeCtrl::Tick(float deltaTime)
{
	float birdX = bird->GetWorldTrafo().pos.x;
	if (birdX - pipes.front()->GetWorldTrafo().pos.x > spawnDist) {
		level->Delete(pipes.front().Get());
		pipes.pop_front();
		level->Delete(pipes.front().Get());
		pipes.pop_front();
	}

	float lastX = pipes.back()->GetWorldTrafo().pos.x;
	if (lastX - birdX < spawnDist) {
		Spawn(Point(lastX + GetRand(xDistance - xDistribution, xDistance + xDistribution), GetRand(-yDistribution, yDistribution)));
	}

	if ((*it)->GetWorldTrafo().pos.x < birdX) {
		++it;
		++it;
		GameState::Instance().AddPoint();
	}
}


void PipeCtrl::CopyContent(GameObject* other)
{
	Sharpheus::Behavior::CopyContent(other);

	PipeCtrl* trueOther = (PipeCtrl*)other;
	spawnDist = trueOther->spawnDist;
	xDistance = trueOther->xDistance;
	xDistribution = trueOther->xDistribution;
	yDistribution = trueOther->yDistribution;
	yDiff = trueOther->yDiff;
}


void PipeCtrl::Fill()
{
	Point center(spawnDist / 2, 0);
	while (center.x < spawnDist) {
		Spawn(center + Point(0, GetRand(-yDistribution, yDistribution)));
		center.x += GetRand(xDistance - xDistribution, xDistance + xDistribution);
	}

	it = pipes.begin();
}


void PipeCtrl::Spawn(const Point& center)
{
	level->AttachSceneTo(parent, "Pipe.scene.sharpheus");
	GameObject* upper = parent->GetLastChild();
	upper->SetTrafo(Transform(center - Point(0, 400 + yDiff / 2), Point(4, -4), 0));
	pipes.push_back(upper);

	level->AttachSceneTo(parent, "Pipe.scene.sharpheus");
	GameObject* lower = parent->GetLastChild();
	lower->SetTrafo(Transform(center + Point(0, 400 + yDiff / 2), Point(4, 4), 0));
	pipes.push_back(lower);
}


float PipeCtrl::GetRand(float rangeBottom, float rangeTop)
{
	return rand() % (int)(rangeTop - rangeBottom) + rangeBottom;
}


void PipeCtrl::Init()
{
	bird = level->GetGameObject("Bird");
	Fill();
	GameState::Instance().Reset();
}


bool PipeCtrl::IsCompatibleWithParent(GameObject* parent)
{
	return parent != nullptr;
}


#include "pch.h"
#include "EnemyControl.hpp"
#include "Engine/Level.hpp"


using namespace Sharpheus;


uint32_t EnemyControl::classVersion = 1;

ClassInfo EnemyControl::classInfo("EnemyControl", "behavior.png", {
	// Provide necessary data members here for the Editor to present them
});


EnemyControl::EnemyControl(Sharpheus::Behavior* other)
	: Sharpheus::Behavior(other), ColliderListener(GetID())
{
}


EnemyControl::EnemyControl(Sharpheus::GameObject* parent, const std::string& name)
	: Sharpheus::Behavior(parent, name), ColliderListener(GetID())
{
}


EnemyControl::~EnemyControl()
{
}


void EnemyControl::TakeDamage(uint32_t dmg)
{
	hp -= (int32_t)dmg;

	if (hp <= 0) {
		level->Delete(parent);
		return;
	}

	if (hpBar) {
		float ratio = (float)hp / maxHp;
		float barWidth = hpBar->GetWidth();
		hpBar->SetTrafo(Transform(Point(-barWidth / 2 * (1 - ratio), 0), Point(ratio, 1), 0));
	}
}


bool EnemyControl::Load(FileLoader& fl)
{
	Sharpheus::Behavior::Load(fl);
	SPH_CHECK_CLASSVERSION(fl, classVersion);
	fl.Read(maxHp);
	fl.Read(dmg);
	fl.Read(attackSpeed);
	return fl.GetStatus();
}


bool EnemyControl::Save(FileSaver& fs)
{
	Sharpheus::Behavior::Save(fs);
	fs.Write(classVersion);
	fs.Write(maxHp);
	fs.Write(dmg);
	fs.Write(attackSpeed);
	return fs.GetStatus();
}


void EnemyControl::Tick(float deltaTime)
{
	if (player) {
		if (isPlayerIn) {
			attackLoopState += deltaTime;
			if (attackLoopState > attackSpeed) {
				attackLoopState = 0.f;
				player->TakeDamage(dmg * GameState::Instance().GetDamageMult());
			}
		}

		if (playerCanDmg && player->Attacks()) {
			TakeDamage(player->GetDMG());
		}
	}
}


void EnemyControl::CopyContent(GameObject* other)
{
	Behavior::CopyContent(other);

	EnemyControl* trueOther = (EnemyControl*)other;
	hp = trueOther->hp;
	maxHp = trueOther->maxHp;
	dmg = trueOther->dmg;
	attackSpeed = trueOther->attackSpeed;
}


void EnemyControl::OnTriggerEnter(const OnEnterEvent& e)
{
	if (player && e.obj->GetID() == player->GetID()) {
		player->TakeDamage(dmg * GameState::Instance().GetDamageMult());
		attackLoopState = 0.f;
		isPlayerIn = true;
	}
}


void EnemyControl::OnPlayerTriggerEnter(const Sharpheus::OnEnterEvent& e)
{
	if (player && e.obj->GetID() == player->GetID()) {
		playerCanDmg = true;
	}
}


void EnemyControl::OnTriggerExit(const OnExitEvent& e)
{
	if (player && e.obj->GetID() == player->GetID()) {
		isPlayerIn = false;
	}
}


void EnemyControl::OnPlayerTriggerExit(const Sharpheus::OnExitEvent& e)
{
	if (player && e.obj->GetID() == player->GetID()) {
		playerCanDmg = false;
	}
}


void EnemyControl::Init()
{
	hp = maxHp;

	player = (PlayerController*)level->GetGameObject("PlayerController");
	hpBar = (Quad*)parent->GetFirstChildOfType(Type::Collection)->GetLastChildOfType(Type::Quad);

	SubscribeOnTriggerEnter((Collider*)parent->GetFirstChildOfMask(TypeMasks::Collider), SPH_BIND(EnemyControl::OnTriggerEnter));
	SubscribeOnTriggerEnter((Collider*)parent->GetLastChildOfMask(TypeMasks::Collider), SPH_BIND(EnemyControl::OnPlayerTriggerEnter));
	SubscribeOnTriggerExit((Collider*)parent->GetFirstChildOfMask(TypeMasks::Collider), SPH_BIND(EnemyControl::OnTriggerExit));
	SubscribeOnTriggerExit((Collider*)parent->GetLastChildOfMask(TypeMasks::Collider), SPH_BIND(EnemyControl::OnPlayerTriggerExit));
}


bool EnemyControl::IsCompatibleWithParent(GameObject* parent)
{
	if (parent == nullptr) {
		return false;
	}

	if (parent->GetLastChildOfMask(TypeMasks::Collider) == nullptr) {
		return false;
	}

	return true;
}


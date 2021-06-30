#pragma once

#include "Behavior.hpp"
#include "LocalListeners/ColliderListener.hpp"
#include "PlayerController.hpp"
#include "Engine/GameObjects/Renderables/Quad.hpp"


class SPH_EXPORT EnemyControl : public Sharpheus::Behavior, public Sharpheus::ColliderListener
{
public:
	EnemyControl(Sharpheus::Behavior* other);
	EnemyControl(Sharpheus::GameObject* parent, const std::string& name);
	virtual ~EnemyControl();

	inline uint32_t GetHP() { return (hp > 0) ? hp : 0; }
	inline uint32_t GetMaxHP() { return maxHp; }
	inline uint32_t GetDMG() { return dmg; }
	inline float GetAttackSpeed() { return attackSpeed; }
	inline void SetMaxHP(uint32_t maxHp) { this->maxHp = maxHp; }
	inline void SetDMG(uint32_t dmg) { this->dmg = dmg; }
	inline void SetAttackSpeed(float attackSpeed) { this->attackSpeed = attackSpeed; }
	
	void TakeDamage(uint32_t dmg);

	virtual bool Load(Sharpheus::FileLoader& fl) override;

	SPH_DECL_BEHAVIOR(EnemyControl, 2);

protected:
	int32_t hp;
	uint32_t maxHp = 100, dmg = 0;
	float attackSpeed = 1.f;
	
	float attackLoopState = 0.f;
	bool isPlayerIn = false, playerCanDmg = false;

	Sharpheus::SafeObject<PlayerController> player;
	Sharpheus::SafeObject<Sharpheus::Quad> hpBar;

	virtual bool Save(Sharpheus::FileSaver& fs) override;

	virtual void Tick(float deltaTime) override;

	virtual void CopyContent(GameObject* other) override;

	void OnTriggerEnter(const Sharpheus::OnEnterEvent& e);
	void OnPlayerTriggerEnter(const Sharpheus::OnEnterEvent& e);
	void OnTriggerExit(const Sharpheus::OnExitEvent& e);
	void OnPlayerTriggerExit(const Sharpheus::OnExitEvent& e);
};


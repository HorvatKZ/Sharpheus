#pragma once

#include "Behavior.hpp"
#include "LocalListeners/ColliderListener.hpp"
#include "Engine/GameObjects/Renderables/AnimationPlayer.hpp"
#include "GameState.hpp"


class SPH_EXPORT PlayerController : public Sharpheus::Behavior, public Sharpheus::ColliderListener
{
public:
	PlayerController(Sharpheus::Behavior* other);
	PlayerController(Sharpheus::GameObject* parent, const std::string& name);
	virtual ~PlayerController();

	inline float GetSpeed() { return speed; }
	inline float GetJumpForce() { return jumpForce; }
	inline uint32_t GetHP() { return (hp > 0) ? hp : 0; }
	inline uint32_t GetMaxHP() { return maxHp; }
	inline uint32_t GetDMG() { return dmg; }
	inline void SetSpeed(float speed) { this->speed = speed; }
	inline void SetJumpForce(float jumpForce) { this->jumpForce = jumpForce; }
	inline void SetMaxHP(uint32_t maxHp) { this->maxHp = maxHp; }
	inline void SetDMG(uint32_t dmg) { this->dmg = dmg; }

	inline void TakeDamage(uint32_t dmg) { hp -= dmg; }
	inline void Heal(uint32_t amount) {
		hp += amount;
		if (hp > maxHp) hp = maxHp;
	}

	inline bool Attacks() {
		if (0.f < attackTimer && attackTimer < 0.3f) {
			bool result = attacks;
			attacks = false;
			return result;
		}

		return false;
	}

	virtual bool Load(Sharpheus::FileLoader& fl) override;

	SPH_DECL_BEHAVIOR(PlayerController, 1);

protected:
	float speed = 0.f, jumpForce = 0.f;
	int32_t hp;
	uint32_t maxHp = 100, dmg = 20;

	bool canJump = false, toRight = true, attacks = false;
	float attackTimer;
	Sharpheus::SafeObject<Sharpheus::AnimationPlayer> upperAnim, lowerAnim;

	virtual bool Save(Sharpheus::FileSaver& fs) override;

	virtual void Tick(float deltaTime) override;

	virtual void CopyContent(GameObject* other) override;

	void OnKeyPressed(const Sharpheus::KeyPressedEvent& e);
	void OnKeyHold(const Sharpheus::KeyHoldEvent& e);
	void OnKeyReleased(const Sharpheus::KeyReleasedEvent& e);
	void OnCollision(const Sharpheus::CollisionEvent& e);

	void Move(const Sharpheus::KeyCode& code);
	void SwitchFacing();
	void PlayMoveAnim();
	void PlayStopAnim();
	void StartAttack();
};


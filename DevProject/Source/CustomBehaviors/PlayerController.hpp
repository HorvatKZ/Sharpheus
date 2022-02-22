#pragma once

#include "CustomMacros.h"
#include "Behavior.hpp"
#include "LocalListeners/ColliderListener.hpp"
#include "Engine/Events/KeyEvents.hpp"
#include "Engine/GameObjects/Renderables/AnimationPlayer.hpp"


class SPH_CUSTOM_EXPORT PlayerController : public Sharpheus::Behavior, public Sharpheus::ColliderListener
{
public:
	PlayerController(Sharpheus::Behavior* other);
	PlayerController(Sharpheus::GameObject* parent, const std::string& name);
	virtual ~PlayerController();

	virtual bool Load(Sharpheus::FileLoader& fl) override;

	inline float GetJumpForce() { return jumpForce; }
	inline float GetSpeed() { return speed; }
	inline void SetJumpForce(float jumpForce) { this->jumpForce = jumpForce; }
	inline void SetSpeed(float speed) { this->speed = speed; }

	SPH_DECL_BEHAVIOR(PlayerController, 2);

protected:
	float jumpForce = 0.f, speed = 0.f;
	bool canJump = false;
	Sharpheus::SafeObject<Sharpheus::AnimationPlayer> anim;

	void Tick(float deltaTime) override;
	virtual bool Save(Sharpheus::FileSaver& fs) override;

	virtual void CopyContent(Sharpheus::GameObject* other) override;

	void OnKeyPressed(const Sharpheus::KeyPressedEvent& e);
	void OnKeyHold(const Sharpheus::KeyHoldEvent& e);
	void OnKeyReleased(const Sharpheus::KeyReleasedEvent& e);
	void OnCollision(const Sharpheus::CollisionEvent& e);
};


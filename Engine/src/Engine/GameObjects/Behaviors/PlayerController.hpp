#pragma once

#include "LocalListenerBehavior.hpp"
#include "Engine/Events/KeyEvents.hpp"


class PlayerController : public Sharpheus::LocalListenerBehavior
{
public:
	PlayerController(GameObject* parent, const std::string& name);
	virtual ~PlayerController();

	void OnKeyPressed(const Sharpheus::KeyPressedEvent& e);
	void OnKeyRepeat(const Sharpheus::KeyRepeatEvent& e);
	void OnKeyReleased(const Sharpheus::KeyReleasedEvent& e);
	void OnCollision(const Sharpheus::CollisionEvent& e);

	inline float GetJumpForce() { return jumpForce; }
	inline float GetSpeed() { return speed; }
	inline void SetJumpForce(float jumpForce) { this->jumpForce = jumpForce; }
	inline void SetSpeed(float speed) { this->speed = speed; }

protected:
	float jumpForce = 0.f, speed = 0.f;
	bool canJump = false;

	void Tick(float deltaTime) override;
};


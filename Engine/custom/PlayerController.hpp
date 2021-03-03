#pragma once

#include "LocalListenerBehavior.hpp"
#include "Engine/Events/KeyEvents.hpp"


class SPH_EXPORT PlayerController : public Sharpheus::LocalListenerBehavior
{
public:
	PlayerController(Sharpheus::Behavior* other);
	PlayerController(Sharpheus::GameObject* parent, const std::string& name);
	virtual ~PlayerController();
	virtual void CopyFrom(Sharpheus::GameObject* other) override;

	virtual bool Load(Sharpheus::FileLoader& fl) override;

	inline float GetJumpForce() { return jumpForce; }
	inline float GetSpeed() { return speed; }
	inline void SetJumpForce(float jumpForce) { this->jumpForce = jumpForce; }
	inline void SetSpeed(float speed) { this->speed = speed; }

	SPH_DECL_BEHAVIOR(PlayerController, 2);

protected:
	float jumpForce = 0.f, speed = 0.f;
	bool canJump = false;

	void Tick(float deltaTime) override;
	void DoSubscriptions();
	virtual bool Save(Sharpheus::FileSaver& fs) override;

	void OnKeyPressed(const Sharpheus::KeyPressedEvent& e);
	void OnKeyRepeat(const Sharpheus::KeyRepeatEvent& e);
	void OnKeyReleased(const Sharpheus::KeyReleasedEvent& e);
	void OnCollision(const Sharpheus::CollisionEvent& e);
};


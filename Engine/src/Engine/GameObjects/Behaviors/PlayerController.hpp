#pragma once

#include "LocalListenerBehavior.hpp"
#include "Engine/Events/KeyEvents.hpp"


class PlayerController : public Sharpheus::LocalListenerBehavior
{
public:
	PlayerController(GameObject* parent, const std::string& name, float jumpForce, float speed);
	virtual ~PlayerController();

	void OnKeyPressed(const Sharpheus::KeyPressedEvent& e);
	void OnKeyRepeat(const Sharpheus::KeyRepeatEvent& e);
	void OnKeyReleased(const Sharpheus::KeyReleasedEvent& e);
	void OnCollision(const Sharpheus::CollisionEvent& e);

protected:
	float jumpForce, speed;
	bool canJump = false;

	void TickThis(float deltaTime) override;
};


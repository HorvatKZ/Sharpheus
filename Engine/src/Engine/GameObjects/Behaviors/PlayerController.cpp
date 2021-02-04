#include "pch.h"
#include "PlayerController.hpp"
#include "Engine/GameObjects/Colliders/PhysicsObject.hpp"


using namespace Sharpheus;


PlayerController::PlayerController(GameObject* parent, const std::string& name, float jumpForce, float speed)
	: LocalListenerBehavior(parent, name), jumpForce(jumpForce), speed(speed)
{
	Subscribe<KeyPressedEvent>(SPH_BIND(PlayerController::OnKeyPressed));
	Subscribe<KeyReleasedEvent>(SPH_BIND(PlayerController::OnKeyReleased));
	SubscribeCollision((Collider*)parent->GetChild("Box"), SPH_BIND(PlayerController::OnCollision));
}


PlayerController::~PlayerController()
{
}

void PlayerController::OnKeyPressed(const Sharpheus::KeyPressedEvent& e)
{
	switch (e.code) {
		case KeyCode::W:
			if (canJump) {
				((PhysicsObject*)parent)->SetVelocityY(-jumpForce);
				canJump = false;
			}
			break;
		case KeyCode::A:
			((PhysicsObject*)parent)->SetVelocityX(-speed);
			break;
		case KeyCode::D:
			((PhysicsObject*)parent)->SetVelocityX(speed);
			break;
	}
}


void PlayerController::OnKeyReleased(const Sharpheus::KeyReleasedEvent& e)
{
	switch (e.code) {
		case KeyCode::A:
		case KeyCode::D:
			((PhysicsObject*)parent)->SetVelocityX(0);
			break;
	}
}

void PlayerController::OnCollision(const Sharpheus::CollisionEvent& e)
{
	if (e.direction == Point::Up) {
		canJump = true;
	}
}


void PlayerController::TickThis(float deltaTime)
{
}

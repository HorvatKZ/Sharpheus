#include "pch.h"
#include "PlayerController.hpp"
#include "Engine/GameObjects/Colliders/PhysicsObject.hpp"


using namespace Sharpheus;

SPH_START_CLASSINFO(PlayerController, "behaviorcpp.png")
SPH_PROVIDE_FLOAT(PlayerController, "Jumpforce", GetJumpForce, SetJumpForce)
SPH_PROVIDE_FLOAT(PlayerController, "Speed", GetSpeed, SetSpeed)
SPH_END_CLASSINFO


PlayerController::PlayerController(Sharpheus::Behavior* other) : Behavior(other), ColliderListener(GetID())
{
}


PlayerController::PlayerController(GameObject* parent, const std::string& name) : Behavior(parent, name), ColliderListener(GetID())
{
}


PlayerController::~PlayerController()
{
}


void PlayerController::CopyContent(GameObject* other)
{
	Behavior::CopyContent(other);

	PlayerController* trueOther = (PlayerController*)other;
	jumpForce = trueOther->jumpForce;
	speed = trueOther->speed;
}


bool PlayerController::Save(FileSaver& fs)
{
	Behavior::Save(fs);
	fs.Write(jumpForce);
	fs.Write(speed);
	return fs.GetStatus();
}


bool PlayerController::Load(FileLoader& fl)
{
	Behavior::Load(fl);
	fl.Read(jumpForce);
	fl.Read(speed);
	return fl.GetStatus();
}


void PlayerController::OnKeyPressed(const Sharpheus::KeyPressedEvent& e)
{
	Transform trafo = anim->GetTrafo();
	switch (e.code) {
	case KeyCode::W:
		if (canJump) {
			((PhysicsObject*)parent)->SetVelocityY(-jumpForce);
			canJump = false;
		}
		break;
	case KeyCode::A:
		((PhysicsObject*)parent)->SetVelocityX(-speed);
		anim->SetCurrentByName("Walk");
		trafo.scale.x = -1;
		anim->SetTrafo(trafo);
		break;
	case KeyCode::D:
		((PhysicsObject*)parent)->SetVelocityX(speed);
		anim->SetCurrentByName("Walk");
		trafo.scale.x = 1;
		anim->SetTrafo(trafo);
		break;
	}
}


void PlayerController::OnKeyHold(const Sharpheus::KeyHoldEvent& e)
{
	switch (e.code) {
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
		anim->SetCurrentByName("Idle");
		break;
	}
}


void PlayerController::OnCollision(const Sharpheus::CollisionEvent& e)
{
	if (e.cd.geom.normal.GetAngleWith(Point::Down) < 10.f) {
		canJump = true;
	}
}


void PlayerController::Tick(float deltaTime)
{
}


void PlayerController::Init()
{
	Subscribe<KeyPressedEvent>(SPH_BIND(PlayerController::OnKeyPressed));
	Subscribe<KeyHoldEvent>(SPH_BIND(PlayerController::OnKeyHold));
	Subscribe<KeyReleasedEvent>(SPH_BIND(PlayerController::OnKeyReleased));
	SubscribeCollision((Collider*)parent->GetFirstChildOfType(Type::CapsuleCollider), SPH_BIND(PlayerController::OnCollision));
	anim = (AnimationPlayer*)parent->GetFirstChildOfType(Type::AnimationPlayer);
}


bool PlayerController::IsCompatibleWithParent(GameObject* parent)
{
	if (parent == nullptr) {
		return false;
	}

	if (!parent->Is(Type::PhysicsObject)) {
		return false;
	}

	if (parent->GetFirstChildOfType(Type::CapsuleCollider) == nullptr) {
		return false;
	}

	if (parent->GetFirstChildOfType(Type::AnimationPlayer) == nullptr) {
		return false;
	}

	return true;
}

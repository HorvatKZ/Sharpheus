#include "pch.h"
#include "PlayerController.hpp"
#include "Engine/GameObjects/Colliders/PhysicsObject.hpp"


using namespace Sharpheus;

ClassInfo PlayerController::classInfo("PlayerController", "behavior.png", {
	new FloatProvider<PlayerController>("Jumpforce", SPH_BIND_GETTER(PlayerController::GetJumpForce), SPH_BIND_SETTER(PlayerController::SetJumpForce)),
	new FloatProvider<PlayerController>("Speed", SPH_BIND_GETTER(PlayerController::GetSpeed), SPH_BIND_SETTER(PlayerController::SetSpeed))
});


PlayerController::PlayerController(Sharpheus::Behavior* other) : LocalListenerBehavior(other)
{
	DoSubscriptions();
}


PlayerController::PlayerController(GameObject* parent, const std::string& name) : LocalListenerBehavior(parent, name)
{
	SPH_ASSERT(parent->Is(Type::PhysicsObject), "Parent \"{0}\" is not a PhysicsObject", parent->GetName());
	DoSubscriptions();
}


PlayerController::~PlayerController()
{
}


void PlayerController::CopyFrom(GameObject* other)
{
	SPH_COPY_HEADER(2);

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


void PlayerController::OnKeyRepeat(const Sharpheus::KeyRepeatEvent& e)
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


void PlayerController::DoSubscriptions()
{
	Subscribe<KeyPressedEvent>(SPH_BIND(PlayerController::OnKeyPressed));
	Subscribe<KeyRepeatEvent>(SPH_BIND(PlayerController::OnKeyRepeat));
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

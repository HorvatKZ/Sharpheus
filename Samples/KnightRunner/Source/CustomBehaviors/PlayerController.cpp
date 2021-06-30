#include "pch.h"
#include "PlayerController.hpp"
#include "Engine/GameObjects/Colliders/PhysicsObject.hpp"


using namespace Sharpheus;


uint32_t PlayerController::classVersion = 1;

ClassInfo PlayerController::classInfo("PlayerController", "behavior.png", {
	new UIntProvider<PlayerController>("MaxHP", SPH_BIND_GETTER(PlayerController::GetMaxHP), SPH_BIND_SETTER(PlayerController::SetMaxHP)),
	new UIntProvider<PlayerController>("Damage", SPH_BIND_GETTER(PlayerController::GetDMG), SPH_BIND_SETTER(PlayerController::SetDMG)),
	new UFloatProvider<PlayerController>("Speed", SPH_BIND_GETTER(PlayerController::GetSpeed), SPH_BIND_SETTER(PlayerController::SetSpeed)),
	new UFloatProvider<PlayerController>("JumpForce", SPH_BIND_GETTER(PlayerController::GetJumpForce), SPH_BIND_SETTER(PlayerController::SetJumpForce))
});


PlayerController::PlayerController(Sharpheus::Behavior* other)
	: Sharpheus::Behavior(other), ColliderListener(GetID())
{
}


PlayerController::PlayerController(Sharpheus::GameObject* parent, const std::string& name)
	: Sharpheus::Behavior(parent, name), ColliderListener(GetID())
{
}


PlayerController::~PlayerController()
{
}


bool PlayerController::Load(FileLoader& fl)
{
	Sharpheus::Behavior::Load(fl);
	SPH_CHECK_CLASSVERSION(fl, classVersion);
	fl.Read(maxHp);
	fl.Read(dmg);
	fl.Read(speed);
	fl.Read(jumpForce);
	return fl.GetStatus();
}


bool PlayerController::Save(FileSaver& fs)
{
	Sharpheus::Behavior::Save(fs);
	fs.Write(classVersion);
	fs.Write(maxHp);
	fs.Write(dmg);
	fs.Write(speed);
	fs.Write(jumpForce);
	return fs.GetStatus();
}


void PlayerController::Tick(float deltaTime)
{
	if (attacks) {
		attackTimer -= deltaTime;
	}

	if (worldTrafo.pos.y > 500) {
		hp = 0;
	}
}


void PlayerController::OnKeyPressed(const KeyPressedEvent& e)
{
	Move(e.code);
}


void PlayerController::OnKeyHold(const KeyHoldEvent& e)
{
	Move(e.code);
}


void PlayerController::OnKeyReleased(const KeyReleasedEvent& e)
{
	switch (e.code) {
		case KeyCode::A: case KeyCode::LEFT:
		case KeyCode::D: case KeyCode::RIGHT:
			((PhysicsObject*)parent)->SetVelocityX(0);
			PlayStopAnim();
			break;
	}
}


void PlayerController::OnCollision(const CollisionEvent& e)
{
	Point collNormal = e.cd.geom.normal;
	if (collNormal.y > 0.8) {
		canJump = true;
	}
}


void PlayerController::Move(const KeyCode& code)
{
	switch (code) {
		case KeyCode::A: case KeyCode::LEFT:
			((PhysicsObject*)parent)->SetVelocityX(-speed);
			if (toRight) {
				SwitchFacing();
			}
			PlayMoveAnim();
			break;
		case KeyCode::D: case KeyCode::RIGHT:
			((PhysicsObject*)parent)->SetVelocityX(speed);
			if (!toRight) {
				SwitchFacing();
			}
			PlayMoveAnim();
			break;
		case KeyCode::W: case KeyCode::UP:
			if (canJump) {
				((PhysicsObject*)parent)->SetVelocityY(-jumpForce);
				canJump = false;
			}
			break;
		case KeyCode::K:
			StartAttack();
			break;
	}
}


void PlayerController::SwitchFacing()
{
	toRight = !toRight;

	if (upperAnim) {
		Transform trafo = upperAnim->GetTrafo();
		trafo.scale.x *= -1;
		upperAnim->SetTrafo(trafo);
	}

	if (lowerAnim) {
		Transform trafo = lowerAnim->GetTrafo();
		trafo.scale.x *= -1;
		lowerAnim->SetTrafo(trafo);
	}
}


void PlayerController::PlayMoveAnim()
{
	if (upperAnim && upperAnim->GetCurrentAnimationName() != "Run_Up") {
		upperAnim->SetCurrentByName("Run_Up");
	}
	if (lowerAnim && lowerAnim->GetCurrentAnimationName() != "Run_Down") {
		lowerAnim->SetCurrentByName("Run_Down");
	}
}


void PlayerController::PlayStopAnim()
{
	if (upperAnim && upperAnim->GetCurrentAnimationName() != "Idle_Up") {
		upperAnim->SetCurrentByName("Idle_Up");
	}
	if (lowerAnim && lowerAnim->GetCurrentAnimationName() != "Idle_Down") {
		lowerAnim->SetCurrentByName("Idle_Down");
	}
}


void PlayerController::StartAttack()
{
	if (upperAnim && upperAnim->GetCurrentAnimationName() != "Attack_Up") {
		upperAnim->PlayOnce("Attack_Up");
		attacks = true;
		attackTimer = 0.6f;
	}
}


void PlayerController::CopyContent(GameObject* other)
{
	Behavior::CopyContent(other);

	PlayerController* trueOther = (PlayerController*)other;
	hp = trueOther->hp;
	maxHp = trueOther->maxHp;
	dmg = trueOther->dmg;
	speed = trueOther->speed;
	jumpForce = trueOther->jumpForce;
}


void PlayerController::Init()
{
	hp = maxHp;

	upperAnim = (AnimationPlayer*)parent->GetChild("PlayerUpper");
	lowerAnim = (AnimationPlayer*)parent->GetChild("PlayerLower");

	Subscribe<KeyPressedEvent>(SPH_BIND(PlayerController::OnKeyPressed));
	Subscribe<KeyHoldEvent>(SPH_BIND(PlayerController::OnKeyHold));
	Subscribe<KeyReleasedEvent>(SPH_BIND(PlayerController::OnKeyReleased));

	SubscribeCollision((Collider*)parent->GetFirstChildOfMask(TypeMasks::Collider), SPH_BIND(PlayerController::OnCollision));
}


bool PlayerController::IsCompatibleWithParent(GameObject* parent)
{
	if (parent == nullptr) {
		return false;
	}

	if (!parent->Is(Type::PhysicsObject)) {
		return false;
	}

	if (parent->GetLastChildOfMask(TypeMasks::Collider) == nullptr) {
		return false;
	}

	return true;
}


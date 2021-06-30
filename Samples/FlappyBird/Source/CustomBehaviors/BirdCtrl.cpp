#include "pch.h"
#include "BirdCtrl.hpp"
#include "Engine/GameObjects/Colliders/PhysicsObject.hpp"
#include "GameState.hpp"


using namespace Sharpheus;


uint32_t BirdCtrl::classVersion = 1;

ClassInfo BirdCtrl::classInfo("BirdCtrl", "behavior.png", {
	new UFloatProvider<BirdCtrl>("JumpForce", SPH_BIND_GETTER(BirdCtrl::GetJumpForce), SPH_BIND_SETTER(BirdCtrl::SetJumpForce))
});


BirdCtrl::BirdCtrl(Sharpheus::Behavior* other)
	: Sharpheus::Behavior(other), ColliderListener(GetID())
{
}


BirdCtrl::BirdCtrl(Sharpheus::GameObject* parent, const std::string& name)
	: Sharpheus::Behavior(parent, name), ColliderListener(GetID())
{
}


BirdCtrl::~BirdCtrl()
{
}


bool BirdCtrl::Load(FileLoader& fl)
{
	Sharpheus::Behavior::Load(fl);
	SPH_CHECK_CLASSVERSION(fl, classVersion);
	fl.Read(jumpForce);
	return fl.GetStatus();
}


bool BirdCtrl::Save(FileSaver& fs)
{
	Sharpheus::Behavior::Save(fs);
	fs.Write(classVersion);
	fs.Write(jumpForce);
	return fs.GetStatus();
}


void BirdCtrl::Tick(float deltaTime)
{
	if (worldTrafo.pos.y > 450 || worldTrafo.pos.y < -450) {
		GameState::Instance().Fail();
	}
}


void BirdCtrl::CopyContent(GameObject* other)
{
	Sharpheus::Behavior::CopyContent(other);

	BirdCtrl* trueOther = (BirdCtrl*)other;
	jumpForce = trueOther->jumpForce;
}


void BirdCtrl::OnClick(const Sharpheus::MousePressedEvent& e)
{
	((PhysicsObject*)parent)->SetVelocityY(-jumpForce);
}


void BirdCtrl::OnCollision(const Sharpheus::CollisionEvent& e)
{
	GameState::Instance().Fail();
}


void BirdCtrl::Init()
{
	Subscribe<MousePressedEvent>(SPH_BIND(BirdCtrl::OnClick));
	SubscribeCollision((Collider*)parent->GetFirstChildOfMask(TypeMasks::Collider), SPH_BIND(BirdCtrl::OnCollision));
}


bool BirdCtrl::IsCompatibleWithParent(GameObject* parent)
{
	return parent != nullptr;
}


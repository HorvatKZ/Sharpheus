#include "pch.h"
#include "Controller.hpp"
#include "Engine/Level.hpp"
#include "../GameState.hpp"


using namespace Sharpheus;

const uint32_t Controller::classVersion = 3;

SPH_START_CLASSINFO(Controller, "behaviorcpp.png")
	SPH_PROVIDE_UFLOAT(Controller, "Speed mult.", GetSpeedMult, SetSpeedMult)
	SPH_PROVIDE_UFLOAT(Controller, "Speed limit", GetSpeedLimit, SetSpeedLimit)
	SPH_PROVIDE_UFLOAT(Controller, "Stop speed", GetStopSpeed, SetStopSpeed)
SPH_END_CLASSINFO


Controller::Controller(Sharpheus::Behavior* other)
	: Sharpheus::Behavior(other), ColliderListener(GetID())
{
}


Controller::Controller(Sharpheus::GameObject* parent, const std::string& name)
	: Sharpheus::Behavior(parent, name), ColliderListener(GetID())
{
}


Controller::~Controller()
{
}


bool Controller::Load(FileLoader& fl)
{
	Sharpheus::Behavior::Load(fl);
	SPH_CHECK_CLASSVERSION(fl, classVersion);
	fl.Read(speedMulitplier);
	fl.Read(speedLimit);
	fl.Read(stopSpeed);
	return fl.GetStatus();
}


bool Controller::Save(FileSaver& fs)
{
	Sharpheus::Behavior::Save(fs);
	fs.Write(classVersion);
	fs.Write(speedMulitplier);
	fs.Write(speedLimit);
	fs.Write(stopSpeed);
	return fs.GetStatus();
}


void Controller::Tick(float deltaTime)
{
	if (GameState::Instance().IsInTurn() && whiteBall && whiteBall->GetVelocity().Length() < stopSpeed) {
		GameState::Instance().EndTurn();
	}

	if (!inPlace && stick && whiteBall) {
		float dist = stick->GetImage()->GetWidth() * stick->GetWorldTrafo().scale.x;
		Point dirUnit = (whiteBall->GetWorldTrafo().pos - mousePos).Normalize();

		Point referencePos = isClicked ? ballPos : mousePos;
		Transform newTrafo(referencePos - dist / 2 * dirUnit, Point(1, 1), dirUnit.GetAngle());
		stick->SetWorldTrafo(newTrafo);
		inPlace = isClicked;
	}
}


void Controller::CopyContent(GameObject* other)
{
	Behavior::CopyContent(other);

	Controller* trueOther = (Controller*)other;
	speedMulitplier = trueOther->speedMulitplier;
	speedLimit = trueOther->speedLimit;
	stopSpeed = trueOther->stopSpeed;
}


void Controller::OnMouseMove(const MouseMovedEvent& e)
{
	mousePos = e.gamePos;
}


void Controller::OnMousePressed(const MousePressedEvent& e)
{
	if (whiteBall && !GameState::Instance().IsInTurn()) {
		ballPos = whiteBall->GetWorldTrafo().pos;
		Point velocity = (ballPos - e.gamePos) * speedMulitplier;
		if (velocity.Length() > speedLimit) {
			velocity = velocity.Normalize() * speedLimit;
		}
		whiteBall->SetVelocity(velocity);
		isClicked = true;
		GameState::Instance().StartTurn();
	}
}


void Controller::OnMouseReleased(const MouseReleasedEvent& e)
{
	isClicked = false;
	inPlace = false;
}


void Controller::OnTriggerEnter(const OnEnterEvent& e)
{
	std::string targetName = e.obj->GetName();
	if (targetName.length() > 5 && targetName.substr(0, 5) == "Ball_") {
		uint32_t ind = std::stoi(targetName.substr(5));
		GameState::Instance().InHole(ind);
		if (ind != 0) {
			level->Delete(e.obj);
		}
		else {
			Transform correctedTrafo = whiteBall->GetWorldTrafo();
			correctedTrafo.pos = startPos;
			whiteBall->SetWorldTrafo(correctedTrafo);
			whiteBall->SetVelocity(Point::Zero);
		}
	}
}


void Controller::OnCollision(const CollisionEvent& e)
{
	std::string targetName = e.with->GetName();
	if (targetName.length() > 6 && targetName.substr(0, 6) == "BallC_") {
		uint32_t ind = std::stoi(targetName.substr(6));
		GameState::Instance().Hit(ind);
	}
}


void Controller::Init()
{
	GameState::Instance().Reset();

	whiteBall = (PhysicsObject*)parent->GetChild("Ball_0");
	stick = (Sprite*)parent->GetChild("Stick");

	if (whiteBall) {
		startPos = whiteBall->GetWorldTrafo().pos;
	}

	Subscribe<MouseMovedEvent>(SPH_BIND(Controller::OnMouseMove));
	Subscribe<MousePressedEvent>(SPH_BIND(Controller::OnMousePressed));
	Subscribe<MouseReleasedEvent>(SPH_BIND(Controller::OnMouseReleased));

	SubscribeCollision((Collider*)whiteBall->GetFirstChildOfMask(TypeMasks::Collider), SPH_BIND(Controller::OnCollision));
	for (uint8_t i = 0; i < 6; ++i) {
		std::string holeName = "Hole_" + std::to_string(i);
		SubscribeOnTriggerEnter((Collider*)level->GetGameObject(holeName), SPH_BIND(Controller::OnTriggerEnter));
	}
}


bool Controller::IsCompatibleWithParent(GameObject* parent)
{
	return parent != nullptr;
}


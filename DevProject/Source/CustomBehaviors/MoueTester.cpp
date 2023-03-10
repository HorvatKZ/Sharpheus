#include "pch.h"
#include "MoueTester.hpp"


using namespace Sharpheus;


SPH_START_CLASSINFO(MoueTester, "behaviorcpp.png")
SPH_END_CLASSINFO


MoueTester::MoueTester(Sharpheus::Behavior* other)
	: Sharpheus::Behavior(other)
{
}


MoueTester::MoueTester(Sharpheus::GameObject* parent, const std::string& name)
	: Sharpheus::Behavior(parent, name)
{
}


MoueTester::~MoueTester()
{
}


bool MoueTester::Load(FileLoader& fl)
{
	Sharpheus::Behavior::Load(fl);
	// Read data members here
	return fl.GetStatus();
}


bool MoueTester::Save(FileSaver& fs)
{
	Sharpheus::Behavior::Save(fs);
	// Write data members here
	return fs.GetStatus();
}


void MoueTester::Tick(float deltaTime)
{
	// Called once per tick
}


void MoueTester::Init()
{
	pressReleaseText = (Text*)parent->GetChild("PressRelease");
	scrollText = (Text*)parent->GetChild("Scroll");
	moveSprite = (Sprite*)parent->GetChild("Move");

	Subscribe<MousePressedEvent>(SPH_BIND(MoueTester::OnMousePressed));
	Subscribe<MouseReleasedEvent>(SPH_BIND(MoueTester::OnMouseReleased));
	Subscribe<MouseMovedEvent>(SPH_BIND(MoueTester::OnMouseMoved));
	Subscribe<MouseScrolledEvent>(SPH_BIND(MoueTester::OnMouseScrolled));
}


void MoueTester::OnMousePressed(const MousePressedEvent& e)
{
	pressReleaseText->SetContent(e.ToStr());
}


void MoueTester::OnMouseReleased(const MouseReleasedEvent& e)
{
	pressReleaseText->SetContent(e.ToStr());
}


void MoueTester::OnMouseMoved(const MouseMovedEvent& e)
{
	Transform trafo = moveSprite->GetWorldTrafo();
	trafo.pos = e.gamePos;
	moveSprite->SetWorldTrafo(trafo);
}


void MoueTester::OnMouseScrolled(const MouseScrolledEvent& e)
{
	scrollText->SetContent(e.ToStr());
}


bool MoueTester::IsCompatibleWithParent(GameObject* parent)
{
	return parent != nullptr;
}


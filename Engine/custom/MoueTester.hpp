#pragma once

#include "Behavior.hpp"
#include "Engine/GameObjects/Renderables/Sprite.hpp"
#include "Engine/GameObjects/Renderables/Text.hpp"


class SPH_EXPORT MoueTester : public Sharpheus::Behavior
{
public:
	MoueTester(Sharpheus::Behavior* other);
	MoueTester(Sharpheus::GameObject* parent, const std::string& name);
	virtual ~MoueTester();

	virtual bool Load(Sharpheus::FileLoader& fl) override;

	SPH_DECL_BEHAVIOR(MoueTester, 4);

protected:
	Sharpheus::SafeObject<Sharpheus::Text> pressReleaseText;
	Sharpheus::SafeObject<Sharpheus::Text> scrollText;
	Sharpheus::SafeObject<Sharpheus::Sprite> moveSprite;

	virtual bool Save(Sharpheus::FileSaver& fs) override;

	virtual void Tick(float deltaTime) override;

	void OnMousePressed(const Sharpheus::MousePressedEvent& e);
	void OnMouseReleased(const Sharpheus::MouseReleasedEvent& e);
	void OnMouseMoved(const Sharpheus::MouseMovedEvent& e);
	void OnMouseScrolled(const Sharpheus::MouseScrolledEvent& e);
};


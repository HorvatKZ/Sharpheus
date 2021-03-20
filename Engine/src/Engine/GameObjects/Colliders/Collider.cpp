#include "pch.h"
#include "Collider.hpp"
#include "Engine/CollisionSystem/CollisionSystem.hpp"
#include "Engine/Level.hpp"


namespace Sharpheus {

	Color Collider::shapeColor = Color(0, 255, 64, 127);


	Collider::Collider(GameObject* parent, const std::string& name, Shape* shape) : ShapedGameObject(parent, name, shape)
	{
		prevPos = worldTrafo.pos;
	}


	Collider::~Collider()
	{
		level->GetCollSys().RemoveCollider(this);

		LocalSourceDestroyedEvent e(GetID());
		for (auto it = subscribers.begin(); it != subscribers.end(); ++it) {
			(*it).second.second(e);
		}
	}


	void Collider::UpdatePrevPos()
	{
		prevPos = worldTrafo.pos;
	}


	void Collider::SetLevel(Level* level)
	{
		GameObject::SetLevel(level);
		level->GetCollSys().AddCollider(this);
	}


	void Collider::Tick(float deltaTime)
	{
	}


	void Collider::Render()
	{
		if (visible) {
			RenderShape();
		}
	}
}
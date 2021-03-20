#include "pch.h"
#include "CircleCollider.hpp"
#include "PhysicsObject.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/CollisionSystem/CollisionEvent.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"


namespace Sharpheus {

	ClassInfo CircleCollider::classInfo("CircleCollider", "circlecollider.png", {
		new UFloatProvider<CircleCollider>("Radius", SPH_BIND_GETTER(CircleCollider::GetRadius), SPH_BIND_SETTER(CircleCollider::SetRadius))
	});


	CircleCollider::CircleCollider(GameObject* parent, const std::string& name)
		: Collider(parent, name, new Oval())
	{
	}


	CircleCollider::~CircleCollider()
	{
	}


	void CircleCollider::CopyFrom(GameObject* other)
	{
		SPH_CHECKTYPE(other, CircleCollider);

		Collider::CopyFrom(other);
		CircleCollider* trueOther = (CircleCollider*)other;
		SetRadius(trueOther->radius);
	}


	std::pair<Point, Point> CircleCollider::CalcCollision(Collider* other)
	{
		switch (other->GetType()) {
		case Type::CircleCollider:
			CircleCollider* otherBox = (CircleCollider*)other;

			if (worldTrafo.pos.Distance(otherBox->worldTrafo.pos) > furthest + otherBox->furthest) {
				return { Point::Zero, Point::Zero };
			}

			uint32_t i = 0;
			/*while (i < 4) {
				uint32_t j = 0;
				while (j < 4) {
					if (Point::DoSectionsIntersect(worldTrafo.pos + offsets[i], worldTrafo.pos + offsets[(i + 1) % 4],
						otherBox->worldTrafo.pos + otherBox->offsets[j], otherBox->worldTrafo.pos + otherBox->offsets[(j + 1) % 4])) {
						return {
							Point::GetNormalVectorToward(worldTrafo.pos + offsets[i],
								worldTrafo.pos + offsets[(i + 1) % 4], otherBox->prevPos),
							Point::GetNormalVectorToward(otherBox->worldTrafo.pos + otherBox->offsets[j],
								otherBox->worldTrafo.pos + otherBox->offsets[(j + 1) % 4], prevPos)
						};
					}
					++j;
				}
				++i;
			}*/
			return { Point::Zero, Point::Zero };
		}
		return { Point::Zero, Point::Zero };
	}


	void CircleCollider::OnCollision(const CollisionEvent& e)
	{
		if (parent != nullptr && parent->GetType() == Type::PhysicsObject) {
			// Setting parent velocity
			Point velocity = ((PhysicsObject*)parent)->GetVelocity();
			float angle = velocity.GetAngle(e.direction);
			bool clockwise = velocity.Rotate(angle).GetAngle(e.direction) < .1f;
			float dirAngle = e.direction.GetAngle(Point::Up);
			if (angle > 90) {
				((PhysicsObject*)parent)->Reflect(angle - 90, clockwise);
			}
			// Correcting position
			Point diff = worldTrafo.pos - prevPos;
			worldTrafo.pos = (worldTrafo.pos.Rotate(clockwise ? dirAngle : -dirAngle) +
				Point(0, -diff.Rotate(clockwise ? dirAngle : -dirAngle).y)).Rotate(clockwise ? -dirAngle : dirAngle);
			parent->SetWorldTrafo(worldTrafo.SubstractSecond(trafo));
		}
		else {
			worldTrafo.pos = prevPos;
			SetWorldTrafo(worldTrafo);
		}

		for (auto it = subscribers.begin(); it != subscribers.end(); ++it) {
			(*it).second.first(e);
		}
	}


	void CircleCollider::RenderSelection()
	{
		ResourceManager::GetCircle()->Render(shape->GetCorners(), selectColor);
	}


	bool CircleCollider::Save(FileSaver& fs)
	{
		GameObject::Save(fs);
		fs.Write(radius);
		return fs.GetStatus();
	}


	bool CircleCollider::Load(FileLoader& fl)
	{
		GameObject::Load(fl);
		fl.Read(radius);
		SetRadius(radius);
		return fl.GetStatus();
	}


	void CircleCollider::RenderShape()
	{
		ResourceManager::GetCircle()->Render(shape->GetCorners(), shapeColor);
	}

}
#include "pch.h"
#include "BoxCollider.hpp"
#include "PhysicsObject.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/CollisionSystem/CollisionEvent.hpp"


namespace Sharpheus {

	BoxCollider::BoxCollider(GameObject* parent, const std::string& name, const Transform& trafo, float width, float height)
		: Collider(parent, name, trafo), width(width), height(height)
	{
		UpdateCorners();
	}

	BoxCollider::~BoxCollider()
	{
	}


	std::pair<Point, Point> BoxCollider::CalcCollision(Collider* other)
	{
		switch (other->GetType()) {
			case Type::BoxCollider:
				BoxCollider* otherBox = (BoxCollider*)other;

				if (worldTrafo.pos.Distance(otherBox->worldTrafo.pos) > radius + otherBox->radius) {
					return { Point::Zero, Point::Zero };
				}

				uint32_t i = 0;
				while (i < 4) {
					uint32_t j = 0;
					while (j < 4) {
						if (Point::DoSectionsIntersect(worldTrafo.pos + cornersOffsets[i], worldTrafo.pos + cornersOffsets[(i + 1) % 4],
							otherBox->worldTrafo.pos + otherBox->cornersOffsets[j], otherBox->worldTrafo.pos + otherBox->cornersOffsets[(j + 1) % 4])) {
							return {
								Point::GetNormalVectorToward(worldTrafo.pos + cornersOffsets[i],
									worldTrafo.pos + cornersOffsets[(i + 1) % 4], otherBox->prevPos),
								Point::GetNormalVectorToward(otherBox->worldTrafo.pos + otherBox->cornersOffsets[j],
									otherBox->worldTrafo.pos + otherBox->cornersOffsets[(j + 1) % 4], prevPos)
								};
						}
						++j;
					}
					++i;
				}
				return { Point::Zero, Point::Zero };
		}
		return { Point::Zero, Point::Zero };
	}


	void BoxCollider::OnCollision(const CollisionEvent& e)
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
		} else {
			worldTrafo.pos = prevPos;
			SetWorldTrafo(worldTrafo);
		}

		for (auto it = subscribers.begin(); it != subscribers.end(); ++it) {
			(*it).second.first(e);
		}
	}


	void BoxCollider::SetTrafo(const Transform& trafo)
	{
		needsToUpdateCorners = needsToUpdateCorners || (this->trafo.scale != trafo.scale || this->trafo.rot != trafo.rot);
		GameObject::SetTrafo(trafo);
	}


	void BoxCollider::TickThis(float deltaTime)
	{
		if (needsToUpdateCorners) {
			UpdateCorners();
		}
	}

	void BoxCollider::RenderShape()
	{
		Renderer::DrawMonocromeQuad(worldTrafo.pos + cornersOffsets[0], worldTrafo.pos + cornersOffsets[1],
			worldTrafo.pos + cornersOffsets[2], worldTrafo.pos + cornersOffsets[3], shapeColor);
	}

	void BoxCollider::UpdateWorldTrafo(const Transform& parentWorldTrafo)
	{
		Point prevScale = worldTrafo.scale;
		float prevRot = worldTrafo.rot;
		GameObject::UpdateWorldTrafo(parentWorldTrafo);
		needsToUpdateCorners = needsToUpdateCorners || (prevScale != worldTrafo.scale || prevRot != worldTrafo.rot);
	}


	void BoxCollider::UpdateCorners()
	{
		cornersOffsets[0] = Point(width * worldTrafo.scale.x / 2, height * worldTrafo.scale.y / 2).Rotate(worldTrafo.rot);
		cornersOffsets[1] = Point(-width * worldTrafo.scale.x / 2, height * worldTrafo.scale.y / 2).Rotate(worldTrafo.rot);
		cornersOffsets[2] = -1 * cornersOffsets[0];
		cornersOffsets[3] = -1 * cornersOffsets[1];
		radius = cornersOffsets[0].Length();
		needsToUpdateCorners = false;
	}
}
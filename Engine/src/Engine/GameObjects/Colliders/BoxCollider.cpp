#include "pch.h"
#include "BoxCollider.hpp"
#include "PhysicsObject.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/CollisionSystem/CollisionEvent.hpp"


namespace Sharpheus {

	ClassInfo BoxCollider::classInfo("BoxCollider", "boxcollider.png", {
		new UFloatProvider<BoxCollider>("Width", SPH_BIND_GETTER(BoxCollider::GetWidth), SPH_BIND_SETTER(BoxCollider::SetWidth)),
		new UFloatProvider<BoxCollider>("Height", SPH_BIND_GETTER(BoxCollider::GetHeight), SPH_BIND_SETTER(BoxCollider::SetHeight))
	});


	SPH_IMPL_COPY(BoxCollider);

	BoxCollider::BoxCollider(GameObject* parent, const std::string& name)
		: Collider(parent, name, true)
	{
		RecalcOffsets();
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


	void BoxCollider::RenderSelection()
	{
		if (needToRecalcOffset) {
			RecalcOffsets();
		}

		RectGameObject::RenderSelection();
	}


	bool BoxCollider::Save(FileSaver& fs)
	{
		GameObject::Save(fs);
		fs.Write(width);
		fs.Write(height);
		return fs.GetStatus();
	}


	bool BoxCollider::Load(FileLoader& fl)
	{
		GameObject::Load(fl);
		fl.Read(width);
		fl.Read(height);
		needToRecalcOffset = true;
		return fl.GetStatus();
	}


	void BoxCollider::RecalcOffsets()
	{
		RecalcOffsetsCommon(width * worldTrafo.scale.x, height * worldTrafo.scale.y, worldTrafo.rot);
		radius = offsets[0].Length();
	}


	void BoxCollider::Tick(float deltaTime)
	{
		if (needToRecalcOffset) {
			RecalcOffsets();
		}
	}

	void BoxCollider::RenderShape()
	{
		if (needToRecalcOffset) {
			RecalcOffsets();
		}

		Renderer::DrawMonocromeQuad(worldTrafo.pos + offsets[0], worldTrafo.pos + offsets[1],
			worldTrafo.pos + offsets[2], worldTrafo.pos + offsets[3], shapeColor);
	}

}
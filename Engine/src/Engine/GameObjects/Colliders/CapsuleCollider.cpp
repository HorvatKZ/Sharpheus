#include "pch.h"
#include "CapsuleCollider.hpp"
#include "PhysicsObject.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/CollisionSystem/CollisionEvent.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"


namespace Sharpheus {

	ClassInfo CapsuleCollider::classInfo("CapsuleCollider", "capsulecollider.png", {
		new UFloatProvider<CapsuleCollider>("Width", SPH_BIND_GETTER(CapsuleCollider::GetWidth), SPH_BIND_SETTER(CapsuleCollider::SetWidth)),
		new UFloatProvider<CapsuleCollider>("Height", SPH_BIND_GETTER(CapsuleCollider::GetHeight), SPH_BIND_SETTER(CapsuleCollider::SetHeight))
		});


	Point CapsuleCollider::upperTexCoords[] = { Point(0, 0), Point(1, 0), Point(1, 0.5), Point(0, 0.5) };
	Point CapsuleCollider::lowerTexCoords[] = { Point(0, 0.5), Point(1, 0.5), Point(1, 1), Point(0, 1) };

	CapsuleCollider::CapsuleCollider(GameObject* parent, const std::string& name)
		: Collider(parent, name, new Capsule())
	{
	}


	CapsuleCollider::~CapsuleCollider()
	{
	}


	void CapsuleCollider::CopyFrom(GameObject* other)
	{
		SPH_CHECKTYPE(other, CapsuleCollider);

		Collider::CopyFrom(other);
		CapsuleCollider* trueOther = (CapsuleCollider*)other;
		width = trueOther->width;
		height = trueOther->height;
		SetSizer(width, height);
	}


	std::pair<Point, Point> CapsuleCollider::CalcCollision(Collider* other)
	{
		switch (other->GetType()) {
		case Type::CapsuleCollider:
			CapsuleCollider* otherBox = (CapsuleCollider*)other;

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


	void CapsuleCollider::OnCollision(const CollisionEvent& e)
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


	void CapsuleCollider::SetWorldTrafo(const Transform& trafo)
	{
		Collider::SetWorldTrafo(trafo);
		UpdateCapsuleDomeHeight();
	}


	void CapsuleCollider::RenderSelection()
	{
		RenderCapsule(selectColor);
	}


	bool CapsuleCollider::Save(FileSaver& fs)
	{
		GameObject::Save(fs);
		fs.Write(width);
		fs.Write(height);
		return fs.GetStatus();
	}


	bool CapsuleCollider::Load(FileLoader& fl)
	{
		GameObject::Load(fl);
		fl.Read(width);
		fl.Read(height);
		SetSizer(width, height);
		return fl.GetStatus();
	}

	void CapsuleCollider::RenderShape()
	{
		RenderCapsule(shapeColor);
	}


	void CapsuleCollider::RenderCapsule(const Color& color)
	{
		Point* corners = shape->GetCorners();
		Point* innerCorners = ((Capsule*)shape)->GetInnerCorners();
		Renderer::DrawMonocromeQuad(innerCorners, color);

		Point upper[] = { corners[0], corners[1], innerCorners[1], innerCorners[0] };
		Point lower[] = { innerCorners[3], innerCorners[2], corners[2], corners[3] };
		ResourceManager::GetCircle()->RenderPart(upper, upperTexCoords, color);
		ResourceManager::GetCircle()->RenderPart(lower, lowerTexCoords, color);
	}


	void CapsuleCollider::UpdateWorldTrafo(const Transform& parentWorldTrafo)
	{
		Collider::UpdateWorldTrafo(parentWorldTrafo);
		UpdateCapsuleDomeHeight();
	}


	void CapsuleCollider::UpdateCapsuleDomeHeight()
	{
		((Capsule*)shape)->SetDomeHeight(this->width * worldTrafo.scale.y / 2);
	}

}
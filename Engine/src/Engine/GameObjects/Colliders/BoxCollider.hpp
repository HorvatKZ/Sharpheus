#pragma once

#include "Collider.hpp"

namespace Sharpheus {

	class SPH_EXPORT BoxCollider : public Collider
	{
	public:
		BoxCollider(GameObject* parent, const std::string& name, const Transform& trafo, float width, float height);
		virtual ~BoxCollider();

		virtual std::pair<Point, Point> CalcCollision(Collider* other) override;
		virtual void OnCollision(const CollisionEvent& e) override;

		virtual void SetTrafo(const Transform& trafo) override;

		inline float GetWidth() { return width; }
		inline float GetHeight() { return height; }
		inline void SetWidth(float width) { this->width = width; needsToUpdateCorners = true; }
		inline void SetHeight(float height) { this->height = height; needsToUpdateCorners = true; }

		SPH_DECL_GAMEOBJECT(BoxCollider)

	protected:
		float width, height;
		Point cornersOffsets[4];
		bool needsToUpdateCorners = true;

		virtual void TickThis(float deltaTime) override;
		virtual void RenderShape() override;

		virtual void UpdateWorldTrafo(const Transform& parentWorldTrafo) override;
		virtual void UpdateCorners();
	};

}
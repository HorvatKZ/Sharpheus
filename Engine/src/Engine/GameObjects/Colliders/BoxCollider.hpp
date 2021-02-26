#pragma once

#include "Collider.hpp"


namespace Sharpheus {

	class SPH_EXPORT BoxCollider : public Collider
	{
	public:
		BoxCollider(GameObject* parent, const std::string& name);
		virtual ~BoxCollider();
		virtual GameObject* Copy() override;

		virtual std::pair<Point, Point> CalcCollision(Collider* other) override;
		virtual void OnCollision(const CollisionEvent& e) override;

		inline float GetWidth() { return width; }
		inline float GetHeight() { return height; }
		inline void SetWidth(float width) { this->width = width; needToRecalcOffset = true; }
		inline void SetHeight(float height) { this->height = height; needToRecalcOffset = true; }
		inline void SetRect(float width, float height) { this->width = width; this->height = height; needToRecalcOffset = true; }

		virtual bool Load(FileLoader& fl) override;

		SPH_DECL_GAMEOBJECT(BoxCollider)

	protected:
		float width = 0.f, height = 0.f;

		virtual bool Save(FileSaver& fs) override;

		virtual void RecalcOffsets() override;
		virtual void Tick(float deltaTime) override;
		virtual void RenderShape() override;
		virtual void RenderSelection() override;
	};

}
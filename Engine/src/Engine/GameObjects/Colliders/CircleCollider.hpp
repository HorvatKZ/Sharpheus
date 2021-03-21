#pragma once

#include "Collider.hpp"


namespace Sharpheus {

	class SPH_EXPORT CircleCollider : public Collider
	{
	public:
		CircleCollider(GameObject* parent, const std::string& name);
		virtual ~CircleCollider();
		virtual void CopyFrom(GameObject* other) override;

		inline float GetRadius() { return radius; }
		inline void SetRadius(float radius) { this->radius = radius; SetSizer(radius, radius); }

		virtual bool Load(FileLoader& fl) override;

		SPH_DECL_GAMEOBJECT(CircleCollider)

	protected:
		float radius = 0.f;
		virtual bool Save(FileSaver& fs) override;

		virtual void RenderShape() override;
		virtual void RenderSelection() override;
	};

}
#pragma once

#include "Collider.hpp"


namespace Sharpheus {

	class SPH_EXPORT CapsuleCollider : public Collider
	{
	public:
		CapsuleCollider(GameObject* parent, const std::string& name);
		virtual ~CapsuleCollider();
		virtual void CopyFrom(GameObject* other) override;

		virtual void SetWorldTrafo(const Transform& trafo) override;

		inline float GetWidth() { return width; }
		inline float GetHeight() { return height; }
		inline void SetWidth(float width) {
			if (width <= height) {
				this->width = width;
			} else {
				this->width = height;
			}
			UpdateCapsuleDomeHeight();
			SetSizer(this->width, this->height);
		}

		inline void SetHeight(float height) {
			if (height >= width) {
				this->height = height;
			} else {
				this->height = width;
			}
			UpdateCapsuleDomeHeight();
			SetSizer(this->width, this->height);
		}

		inline void SetRect(float width, float height) { SetWidth(width); SetHeight(height); }

		virtual bool Load(FileLoader& fl) override;

		SPH_DECL_GAMEOBJECT(CapsuleCollider)

	protected:
		float width = 0.f, height = 0.f;
		Point innerOffsets[4];

		static Point upperTexCoords[4];
		static Point lowerTexCoords[4];

		virtual bool Save(FileSaver& fs) override;

		virtual void RenderShape() override;
		virtual void RenderSelection() override;
		virtual void RenderCapsule(const Color& color);

		virtual void UpdateWorldTrafo(const Transform& parentWorldTrafo) override;
		virtual void UpdateCapsuleDomeHeight();
	};

}
#pragma once

#include "Shape.hpp"


namespace Sharpheus {

	class Capsule : public Shape
	{
	public:
		Capsule() = default;
		Capsule(const Point& pos, const Point& dim, float rot);
		virtual ~Capsule();

		virtual inline Type GetType() override { return Type::CAPSULE; }
		virtual bool IsInside(const Point& p) override;
		virtual Point GetLocalPerpendicularAt(const Point& surfaceP) override;
		virtual Point GetLocalClosestTo(const Point& p) override;

		inline Point* GetInnerCorners() { CheckInnerCorners(); return innerCorners; }

		virtual void ForceRefresh() override;

		inline float GetDomeHeight() { return domeHeight; }

		virtual inline void SetPos(const Point& pos) override {
			if (pos != this->pos) {
				this->pos = pos;
				needsToRecalc = true;
				needsToRecalcInner = true;
			}
		}

		inline void SetDim(const Point& dim) override {
			if (dim != this->dim) {
				this->dim = dim;
				furthest = dim.x > dim.y ? dim.x : dim.y;
				if (domeHeight > dim.y) {
					domeHeight = dim.y;
				}
				needsToRecalc = true;
				needsToRecalcInner = true;
			}
		}

		virtual inline void SetRot(float rot) override {
			if (rot != this->rot) {
				this->rot = rot;
				RecalcAxes();
				needsToRecalc = true;
				needsToRecalcInner = true;
			}
		}

		virtual inline void SetDomeHeight(float domeHeight) {
			if (domeHeight != this->domeHeight) {
				this->domeHeight = domeHeight < dim.y ? domeHeight : dim.y;
				needsToRecalcInner = true;
			}
		}

		virtual Intersection GetIntersectionWith(class Oval* other) override;
		virtual Intersection GetIntersectionWith(class Rect* other) override;
		virtual Intersection GetIntersectionWith(class Capsule* other) override;

	protected:
		Point innerCorners[4];
		float domeHeight = 0.f;
		bool needsToRecalcInner = true;

		void CheckInnerCorners();
	};

}
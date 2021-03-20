#pragma once

#include "Shape.hpp"


namespace Sharpheus {

	class Capsule : public Shape
	{
	public:
		Capsule() = default;
		Capsule(const Point& pos, const Point& dim, float rot);
		virtual ~Capsule();

		virtual bool IsInside(const Point& p);

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

		CollData GetCollWith(const class Oval& other);
		CollData GetCollWith(const class Rect& other);
		CollData GetCollWith(const class Capsule& other);

	protected:
		Point innerCorners[4];
		float domeHeight = 0.f;
		bool needsToRecalcInner = true;

		void CheckInnerCorners();
	};

}
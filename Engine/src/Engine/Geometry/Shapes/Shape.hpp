#pragma once

#include "../Point.hpp"


namespace Sharpheus {

	class Shape
	{
	public:
		enum class Type {
			OVAL, RECT, CAPSULE
		};

		struct Intersection {
			Point contact, normal;
			float depth = 0.f;
		};

		Shape();
		Shape(const Point& pos, const Point& dim, float rot);
		virtual ~Shape();

		virtual inline Type GetType() = 0;
		virtual bool IsInside(const Point& p) = 0;
		virtual Point GetLocalPerpendicularAt(const Point& surfaceP) = 0;
		virtual Point GetLocalClosestTo(const Point& p) = 0;

		virtual inline uint8_t GetPriority() { return 0; }

		inline Point* GetCorners() { CheckCorners(); return corners; }
		virtual inline Point* GetSATCorners() { return GetCorners(); }
		virtual inline uint8_t GetSATCornerNum() { return 4; }
		virtual inline bool IsSATSymmetrical() { return true; }

		virtual void ForceRefresh();

		inline bool IsTooFarFrom(Shape* other) { return IsTooFarFrom(*other); }
		inline bool IsTooFarFrom(const Shape& other) {
			float closestDist = furthest + other.furthest;
			return pos.DistanceSquared(other.pos) > closestDist * closestDist;
		}

		inline const Point& GetPos() { return pos; }
		inline const Point& GetXAxis() { return xAxis; }
		inline const Point& GetYAxis() { return yAxis; }
		inline const Point& GetDim() { return dim; }
		inline float GetRot() { return rot; }

		Point GetRelativePos(const Point& p);

		virtual inline void SetPos(const Point& pos) {
			if (pos != this->pos) {
				this->pos = pos;
				needsToRecalc = true;
			}
		}

		virtual inline void SetDim(const Point& dim) {
			if (dim != this->dim) {
				this->dim = dim;
				UpdateFurthest();
				needsToRecalc = true;
			}
		}

		virtual inline void SetRot(float rot) {
			if (rot != this->rot) {
				this->rot = rot;
				RecalcAxes();
				needsToRecalc = true;
			}
		}

		virtual Intersection GetIntersectionWith(Shape* other);

	protected:
		struct IntersectionData {
			Point contactPoints[2], normal;
			float smallestContact = -1.f;
			bool use2 = false, fromOther = false;
		};

		Point xAxis, yAxis;
		Point pos, dim;
		Point corners[4];
		float rot, furthest;
		bool needsToRecalc = true;

		void RecalcAxes();
		virtual void CheckCorners();
		virtual void UpdateFurthest() = 0;

		IntersectionData GetIntersectionDataWithSAT(Shape* other);
		virtual IntersectionData GetOneWayIntersectionDataWithSAT(Shape* other);
	};

}
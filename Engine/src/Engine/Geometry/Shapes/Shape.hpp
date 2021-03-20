#pragma once

#include "../CollData.hpp"


namespace Sharpheus {

	class Shape
	{
	public:
		Shape();
		Shape(const Point& pos, const Point& dim, float rot);
		virtual ~Shape();

		virtual bool IsInside(const Point& p) = 0;

		inline Point* GetCorners() { CheckCorners(); return corners; }

		virtual void ForceRefresh();

		inline bool IsTooFarFrom(Shape* other) { return IsTooFarFrom(*other); }
		inline bool IsTooFarFrom(const Shape& other) {
			float closestDist = furthest * other.furthest;
			return pos.DistanceSquared(other.pos) > closestDist * closestDist;
		}

		inline const Point& GetPos() { return pos; }
		inline const Point& GetXAxis() { return xAxis; }
		inline const Point& GetYAxis() { return yAxis; }
		inline const Point& GetDim() { return dim; }
		inline float GetRot() { return rot; }

		virtual inline void SetPos(const Point& pos) {
			if (pos != this->pos) {
				this->pos = pos;
				needsToRecalc = true;
			}
		}

		virtual inline void SetDim(const Point& dim) {
			if (dim != this->dim) {
				this->dim = dim;
				furthest = dim.x > dim.y ? dim.x : dim.y;
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

	protected:
		Point xAxis, yAxis;
		Point pos, dim;
		Point corners[4];
		float rot, furthest;
		bool needsToRecalc = true;

		void RecalcAxes();
		void CheckCorners();
		Point GetRelativePos(const Point& p);
	};

}
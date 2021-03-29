#pragma once

#include "Shape.hpp"

namespace Sharpheus {

	class Rect : public Shape
	{
	public:
		Rect() = default;
		Rect(const Point& pos, const Point& dim, float rot);
		virtual ~Rect();

		virtual inline Type GetType() override { return Type::RECT; }
		virtual bool IsInside(const Point& p) override;
		virtual Point GetLocalPerpendicularAt(const Point& surfaceP) override;
		virtual Point GetLocalClosestTo(const Point& p) override;

	protected:
		virtual void UpdateFurthest() override;

		virtual IntersectionData GetOneWayIntersectionDataWithSAT(Shape* other) override;
	};

}
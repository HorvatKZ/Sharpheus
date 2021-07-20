#pragma once

#include "Shape.hpp"


namespace Sharpheus {

	class Oval : public Shape
	{
	public:
		Oval() = default;
		Oval(const Point& pos, const Point& dim, float rot);
		virtual ~Oval();

		inline bool IsCircle() { return dim.x == dim.y; }

		virtual inline Type GetType() override { return Type::OVAL; }
		virtual bool IsInside(const Point& p) override;
		virtual Point GetLocalPerpendicularAt(const Point& surfaceP) override;
		virtual Point GetLocalClosestTo(const Point& p) override;

		virtual uint8 GetPriority() { return IsCircle() ? 2 : 1; }

		virtual inline Point* GetSATCorners() override { CheckCorners(); return satCorners; }
		virtual inline uint8 GetSATCornerNum() override { return 8; }

		virtual Intersection GetIntersectionWith(Shape* other) override;

	protected:
		Point satCorners[8];

		virtual void CheckCorners() override;
		virtual void UpdateFurthest() override;

		virtual Intersection GetIntersectionAsCircleWith(Shape* other);
		virtual Intersection GetIntersectionAsCircleWithCircle(class Oval* other);
		virtual Intersection GetIntersectionAsCircleFromClosest(Shape* other, const Point& closest, const Point& center);
	};

}
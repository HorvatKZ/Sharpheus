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

		virtual inline Intersection GetIntersectionWith(Shape* other) override {
			if (IsCircle()) {
				return GetIntersectionAsCircleWith(other);
			}
			
			return Shape::GetIntersectionWith(other);
		}

	protected:
		virtual Intersection GetIntersectionWith(class Oval* other) override;
		virtual Intersection GetIntersectionWith(class Rect* other) override;
		virtual Intersection GetIntersectionWith(class Capsule* other) override;

		virtual Intersection GetIntersectionAsCircleWith(Shape* other);
		virtual Intersection GetIntersectionAsCircleWithCircle(class Oval* other);
		virtual Intersection GetIntersectionAsCircleFromClosest(Shape* other, const Point& closest, const Point& center);
	};

}
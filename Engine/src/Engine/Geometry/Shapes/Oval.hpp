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

		virtual inline Intersection GetIntersectionWith(Shape* other) override {
			if (IsCircle()) {
				switch (other->GetType()) {
				case Type::OVAL:
					return GetIntersectionAsCircleWith((class Oval*)other);
				case Type::RECT:
					return GetIntersectionAsCircleWith((class Rect*)other);
				case Type::CAPSULE:
					return GetIntersectionAsCircleWith((class Capsule*)other);
				}
			}
			
			return Shape::GetIntersectionWith(other);
		}

	protected:
		virtual Intersection GetIntersectionWith(class Oval* other) override;
		virtual Intersection GetIntersectionWith(class Rect* other) override;
		virtual Intersection GetIntersectionWith(class Capsule* other) override;

		virtual Intersection GetIntersectionAsCircleWith(class Oval* other);
		virtual Intersection GetIntersectionAsCircleWith(class Rect* other);
		virtual Intersection GetIntersectionAsCircleWith(class Capsule* other);

		virtual Intersection GetIntersectionAsCircleWithCircle(class Oval* other);
		virtual Intersection GetIntersectionAsInside(class Oval* other, const Point& center);
		virtual Intersection GetIntersectionAsInside(class Rect* other, const Point& center);

		virtual Intersection GetIntersectionAsCircleFromClosest(class Shape* other, const Point& closest, const Point& center);
	};

}
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
		struct IntersectionData {
			Point contactPoints[2], normal;
			float smallestContact = -1.f;
			bool use2;
		};

		virtual Intersection GetIntersectionWith(class Oval* other) override;
		virtual Intersection GetIntersectionWith(class Rect* other) override;
		virtual Intersection GetIntersectionWith(class Capsule* other) override;

		virtual IntersectionData GetIntesectionDataWith(Rect* other);
		virtual void UpdateSmallestContact(float diff, const Point& newP, const Point& newNormal, IntersectionData& data);
	};

}
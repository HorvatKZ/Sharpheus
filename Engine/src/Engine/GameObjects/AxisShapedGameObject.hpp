#pragma once

#include "../ShapedGameObject.hpp"


namespace Sharpheus {

	class SPH_EXPORT AxisShapedGameObject : public ShapedGameObject
	{
	public:
		AxisShapedGameObject(GameObject* parent, const std::string& name, Shape* shape);
		virtual ~AxisShapedGameObject() = default;
		virtual void CopyFrom(GameObject* other) override;

		virtual void SetWorldTrafo(const Transform& trafo) override;
		
		inline const Point& GetXAxis() { return xAxis; }
		inline const Point& GetYAxis() { return yAxis; }

	protected:
		Point xAxis = Point(1, 0), yAxis = Point(0, 1);

		virtual void UpdateWorldTrafo(const Transform& parentWorldTrafo) override;
		virtual void UpdateSizer() = 0;
		void UpdateAxes(const Transform& oldTrafo);
		void ForceUpdateAxes();
	};

}
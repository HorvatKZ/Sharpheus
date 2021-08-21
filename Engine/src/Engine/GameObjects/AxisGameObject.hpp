#pragma once

#include "../RenderableGameObject.hpp"


namespace Sharpheus {

	class SPH_EXPORT AxisGameObject : public RenderableGameObject
	{
	public:
		AxisGameObject(GameObject* parent, const std::string& name, Shape* shape);
		virtual ~AxisGameObject() = default;
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
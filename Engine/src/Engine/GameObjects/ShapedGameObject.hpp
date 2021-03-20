#pragma once

#include "GameObject.hpp"


namespace Sharpheus {

	class SPH_EXPORT ShapedGameObject : public GameObject
	{
	public:
		ShapedGameObject(GameObject* parent, const std::string& name, Shape* shape);
		virtual ~ShapedGameObject();

		virtual inline Shape* GetShape() { return shape; }

		virtual void SetWorldTrafo(const Transform & trafo) override;

	protected:
		Shape* shape = nullptr;

		inline void SetSizer(float dimX, float dimY) { SetSizer(Point(dimX, dimY)); }
		inline void SetSizer(const Point& sizer) {
			if (sizer != this->sizer) {
				this->sizer = sizer;
				UpdateShape();
			}
		}

		virtual void RenderSelection() override;
		virtual bool IsSelected(const Point& pos) override;

		virtual void UpdateWorldTrafo(const Transform & parentWorldTrafo) override;
		virtual void UpdateShape();

	private:
		Point sizer;
	};

}
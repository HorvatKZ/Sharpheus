#pragma once

#include "GameObject.hpp"


namespace Sharpheus {

	class SPH_EXPORT RectGameObject : public GameObject
	{
	public:
		RectGameObject(GameObject* parent, const std::string& name, bool useRect = true);
		virtual ~RectGameObject() = default;

		virtual void SetTrafo(const Transform & trafo) override;
		virtual void SetWorldTrafo(const Transform & trafo) override;

		virtual inline void SetNeedToRecalc(bool value) { needToRecalcOffset = value; }

	protected:
		Point offsets[4];
		bool needToRecalcOffset = true;
		bool useRect;

		virtual void RecalcOffsets() = 0;
		virtual void RecalcOffsetsCommon(float absoluteWidth, float absoluteHeight, float angle);
		virtual void RenderSelection() override;
		virtual bool IsSelected(const Point& pos) override;

		virtual void UpdateWorldTrafo(const Transform & parentWorldTrafo) override;
	};

}
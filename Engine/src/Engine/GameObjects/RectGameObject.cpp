#include "pch.h"
#include "RectGameObject.hpp"
#include "Engine/Renderer/Renderer.hpp"


namespace Sharpheus {

	RectGameObject::RectGameObject(GameObject* parent, const std::string& name, bool useRect)
		: GameObject(parent, name), useRect(useRect)
	{
	}


	void RectGameObject::SetTrafo(const Transform& trafo)
	{
		needToRecalcOffset = needToRecalcOffset || (this->trafo.scale != trafo.scale || this->trafo.rot != trafo.rot);
		GameObject::SetTrafo(trafo);
	}


	void RectGameObject::SetWorldTrafo(const Transform& trafo)
	{
		needToRecalcOffset = needToRecalcOffset || (worldTrafo.scale != trafo.scale || worldTrafo.rot != trafo.rot);
		GameObject::SetWorldTrafo(trafo);
	}


	void RectGameObject::RecalcOffsetsCommon(float absoluteWidth, float absoluteHeight, float angle)
	{
		offsets[0] = Point(-absoluteWidth / 2, -absoluteHeight / 2).Rotate(angle);
		offsets[1] = Point(absoluteWidth / 2, -absoluteHeight / 2).Rotate(angle);
		offsets[2] = -1 * offsets[0];
		offsets[3] = -1 * offsets[1];
		needToRecalcOffset = false;
	}


	void RectGameObject::RenderSelection()
	{
		if (needToRecalcOffset) {
			RecalcOffsets();
		}

		if (useRect) {
			Renderer::DrawMonocromeQuad(worldTrafo.pos + offsets[0], worldTrafo.pos + offsets[1],
				worldTrafo.pos + offsets[2], worldTrafo.pos + offsets[3], selectColor);
		} else {
			GameObject::RenderSelection();
		}
	}

	bool RectGameObject::IsSelected(const Point& pos)
	{
		if (useRect) {
			if (needToRecalcOffset) {
				RecalcOffsets();
			}

			if (worldTrafo.rot == 0) {
				return (worldTrafo.pos + offsets[0]).x <= pos.x && pos.x <= (worldTrafo.pos + offsets[1]).x
					&& (worldTrafo.pos + offsets[0]).y <= pos.y && pos.y <= (worldTrafo.pos + offsets[3]).y;
			}

			uint32_t i = 0;
			while (i < 4) {
				if (Point::IsBelow(worldTrafo.pos + offsets[i], worldTrafo.pos + offsets[(i + 1) % 4], pos) != 
					Point::IsBelow(worldTrafo.pos + offsets[i], worldTrafo.pos + offsets[(i + 1) % 4], worldTrafo.pos + offsets[(i + 2) % 4])) {
					return false;
				}
				++i;
			}

			return true;
		}

		return GameObject::IsSelected(pos);
	}


	void RectGameObject::UpdateWorldTrafo(const Transform& parentWorldTrafo)
	{
		Point prevScale = worldTrafo.scale;
		float prevRot = worldTrafo.rot;
		GameObject::UpdateWorldTrafo(parentWorldTrafo);
		needToRecalcOffset = needToRecalcOffset || (prevScale != worldTrafo.scale || prevRot != worldTrafo.rot);
	}
}

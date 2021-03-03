#include "editor_pch.h"
#include "EditingArrow.hpp"


namespace Sharpheus {

	EditingArrow::EditingArrow(Camera* camera) : sprite(new Sprite(nullptr, "EditingArrow")), camera(camera)
	{
		sprite->SetImageFromPath("Editor\\editing_arrow.png", true);
	}


	EditingArrow::~EditingArrow()
	{
		delete sprite;
	}


	void EditingArrow::Render(const Transform& trafo)
	{
		this->trafo = trafo;
		sprite->SetWorldTrafo(trafo);
		sprite->RenderAll();
	}

	EditingArrow::EditType EditingArrow::ClickOn(const Point& clickPos)
	{
		Point relativePos = camera->ScreenPosToGamePos(clickPos) - trafo.pos;
		Point camScale = camera->GetWorldTrafo().scale;
		relativePos = Point(relativePos.x / camScale.x, relativePos.y / camScale.y);
		relativePos = relativePos.Rotate(-trafo.rot);
		float len = relativePos.Length();
		if (len <= 9.f) {
			return EditType::POS;
		}
		else if (relativePos.x < 0 && relativePos.y > 0 && len < 30.f) {
			return EditType::ROT;
		}
		else if ((relativePos - Point(28, -28)).Length() < 7.f) {
			return EditType::SCALE;
		}
		else if (abs(relativePos.x) <= 4 && relativePos.y < 0) {
			return EditType::POS_Y;
		}
		else if (abs(relativePos.y) <= 4 && relativePos.x > 0) {
			return EditType::POS_X;
		} 
		else if (abs(relativePos.y + 28) < 5.f && relativePos.x > 0 && relativePos.x < 28) {
			return EditType::SCALE_Y;
		}
		else if (abs(relativePos.x - 28) < 5.f && relativePos.y < 0 && relativePos.y > -28) {
			return EditType::SCALE_X;
		}
		return EditType::NONE;
	}

}
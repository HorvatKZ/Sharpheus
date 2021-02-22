#pragma once

#include "editor_pch.h"

namespace Sharpheus {

	class EditingArrow
	{
	public:
		enum class EditType {
			NONE,
			POS, POS_X, POS_Y,
			SCALE, SCALE_X, SCALE_Y,
			ROT
		};

		EditingArrow(Camera* camera);
		virtual ~EditingArrow();

		void Render(const Transform& trafo);

		EditType ClickOn(const Point& clickPos);

	private:
		Sprite* sprite;
		Camera* camera;
		Transform trafo;
	};

}
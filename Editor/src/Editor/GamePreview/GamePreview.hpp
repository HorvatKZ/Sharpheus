#pragma once

#include "editor_pch.h"


namespace Sharpheus {

	class GamePreview : public GameBase
	{
	public:
		GamePreview(Level* level);
		virtual ~GamePreview();

		void WindowClosed(const WindowClosedEvent& e) override;
	};

}
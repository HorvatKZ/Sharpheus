#pragma once

#include "editor_pch.h"
#include "Engine/Window/Window.hpp"
#include "PreviewCanvas.hpp"


namespace Sharpheus {

	class GamePreview : public GameBase, public wxFrame
	{
	public:
		GamePreview(wxWindow* parent, const std::string& levelFilePath, wxGLContext* glContext, const Window::Props& winProps);
		virtual ~GamePreview();

		void OnIdle(wxIdleEvent& e);

		void WindowClosed(const WindowClosedEvent& e) override;

	private:
		PreviewCanvas* canvas;
		wxLongLong lastTick;

		void OnResize(wxSizeEvent& e);
	};

}
#pragma once

#include "editor_pch.h"
#include "Engine/Window/Window.hpp"
#include "PreviewCanvas.hpp"


namespace Sharpheus {

	class GamePreview : public GameBase, public wxFrame
	{
	public:
		GamePreview(wxWindow* parent, const std::string& projectPath, wxGLContext* glContext, const Window::Props& winProps);
		virtual ~GamePreview();

		void OnIdle(wxIdleEvent& e);

		void WindowClosed(const WindowClosedEvent& e) override;

	private:
		PreviewCanvas* canvas;
		wxLongLong lastTick;
		std::unordered_set<int> pressedKeys;

		void OnResize(wxSizeEvent& e);
		void OnClose(wxCloseEvent& e);
		void OnKeyDown(wxKeyEvent& e);
		void OnKeyUp(wxKeyEvent& e);
	};

}
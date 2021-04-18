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

		static wxPoint shift;

		void OnResize(wxSizeEvent& e);
		void OnClose(wxCloseEvent& e);
		void OnKeyDown(wxKeyEvent& e);
		void OnKeyUp(wxKeyEvent& e);
		void OnLeftDown(wxMouseEvent& e);
		void OnLeftUp(wxMouseEvent& e);
		void OnRightDown(wxMouseEvent& e);
		void OnRightUp(wxMouseEvent& e);
		void OnMiddleDown(wxMouseEvent& e);
		void OnMiddleUp(wxMouseEvent& e);
		void On4Down(wxMouseEvent& e);
		void On4Up(wxMouseEvent& e);
		void On5Down(wxMouseEvent& e);
		void On5Up(wxMouseEvent& e);
		void OnMove(wxMouseEvent& e);
		void OnScroll(wxMouseEvent& e);

		void OnMousePressed(MouseButton button);
		void OnMouseReleased(MouseButton button);
	};

}
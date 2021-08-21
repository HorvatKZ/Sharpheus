#pragma once

#include "editor_pch.h"
#include <GL/glew.h>
#include <GL/wglew.h>
#include <wx/glcanvas.h>
#include "ViewPortCamera.hpp"


namespace Sharpheus {

	class ViewPortBase : public wxGLCanvas
	{
	public:
		ViewPortBase(wxFrame* parent, const wxPoint& pos, const wxSize& size);
		virtual ~ViewPortBase();

		void BindCallbacks(std::function<void()>&& currTrafoChangedCallback);

		void OnPaintEvent(wxPaintEvent& evt);
		void PaintNow();

		inline wxGLContext* GetContext() { return glContext; }

	protected:
		Collection* commonParent;
		ViewPortCamera* camera;
		wxGLContext* glContext = nullptr;

		wxPoint prevMousePos;
		std::function<void()> currTrafoChangedCallback;

		virtual void OnResize(wxSizeEvent& e);
		virtual void OnScroll(wxMouseEvent& e);
		virtual void OnMouseMove(wxMouseEvent& e);
		
		virtual void Render(wxDC& dc) = 0;
	};

}
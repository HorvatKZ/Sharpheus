#pragma once

#include "editor_pch.h"
#include <GL/glew.h>
#include <GL/wglew.h>
#include <wx/glcanvas.h>


namespace Sharpheus {

	class ViewPort : public wxGLCanvas
	{
	public:
		ViewPort(wxFrame* parent, const wxPoint& pos, const wxSize& size);
		virtual ~ViewPort();

		void OnPaintEvent(wxPaintEvent& evt);
		void PaintNow();

	private:
		void Render(wxDC& dc);

		wxGLContext* glContext = nullptr;

		DECLARE_EVENT_TABLE()
	};

}
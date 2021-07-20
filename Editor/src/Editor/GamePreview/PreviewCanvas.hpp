#pragma once

#include "editor_pch.h"
#include <wx/glcanvas.h>


namespace Sharpheus {

	class PreviewCanvas : public wxGLCanvas
	{
	public:
		PreviewCanvas(wxWindow* parent, wxGLContext* other, Project* proj);
		virtual ~PreviewCanvas();

		void OnPaintEvent(wxPaintEvent& evt);
		void PaintNow();

	private:
		wxGLContext* glContext = nullptr;
		Project* proj;
		
		static const Point renderShift;

		void Render(wxDC& dc);
	};

}
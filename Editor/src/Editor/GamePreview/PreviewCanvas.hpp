#pragma once

#include "editor_pch.h"
#include <wx/glcanvas.h>


namespace Sharpheus {

	class PreviewCanvas : public wxGLCanvas
	{
	public:
		PreviewCanvas(wxWindow* parent, wxGLContext* other, Level* level);
		virtual ~PreviewCanvas();

		void OnPaintEvent(wxPaintEvent& evt);
		void PaintNow();

	private:
		wxGLContext* glContext = nullptr;
		Level* level;

		void Render(wxDC& dc);
	};

}
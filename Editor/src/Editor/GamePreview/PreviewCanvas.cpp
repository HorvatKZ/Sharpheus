#include "editor_pch.h"
#include "PreviewCanvas.hpp"


namespace Sharpheus {

	PreviewCanvas::PreviewCanvas(wxWindow* parent, wxGLContext* other, Level* level)
		: wxGLCanvas(parent, wxID_ANY), level(level)
	{
		glContext = new wxGLContext(this, other);
		SetBackgroundStyle(wxBG_STYLE_CUSTOM);

		wxGLCanvas::SetCurrent(*glContext);

		Bind(wxEVT_PAINT, &PreviewCanvas::OnPaintEvent, this);
	}


	PreviewCanvas::~PreviewCanvas()
	{
		delete glContext;
	}


	void PreviewCanvas::OnPaintEvent(wxPaintEvent& evt)
	{
		wxPaintDC dc(this);
		Render(dc);
	}


	void PreviewCanvas::PaintNow()
	{
		wxClientDC dc(this);
		Render(dc);
	}


	void PreviewCanvas::Render(wxDC& dc)
	{
		Renderer::StartFrame();

		level->Render();

		Renderer::EndFrame();
		SwapBuffers();
	}
}

#include "editor_pch.h"
#include "PreviewCanvas.hpp"


namespace Sharpheus {

	const Point PreviewCanvas::renderShift(-8, 8);


	PreviewCanvas::PreviewCanvas(wxWindow* parent, wxGLContext* other, Project* proj)
		: wxGLCanvas(parent, wxID_ANY), proj(proj)
	{
		glContext = new wxGLContext(this, other);
		SetBackgroundStyle(wxBG_STYLE_CUSTOM);

		wxGLCanvas::SetCurrent(*glContext);
		Renderer::UseSecondary();

		Bind(wxEVT_PAINT, &PreviewCanvas::OnPaintEvent, this);
	}


	PreviewCanvas::~PreviewCanvas()
	{
		delete glContext;
		Renderer::UsePrimary();
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
		//wxGLCanvas::SetCurrent(*glContext);
		Renderer::StartFrame(renderShift);

		proj->Render();

		Renderer::EndFrame();
		SwapBuffers();
	}
}

#include "editor_pch.h"
#include "ViewPort.hpp"
#include "Editor/EditorData.hpp"


BEGIN_EVENT_TABLE(Sharpheus::ViewPort, wxPanel)
	EVT_PAINT(OnPaintEvent)
END_EVENT_TABLE()


namespace Sharpheus {

	ViewPort::ViewPort(wxFrame* parent, const wxPoint& pos, const wxSize& size)
		: wxGLCanvas(parent, wxID_ANY, NULL, pos, size)
	{
		glContext = new wxGLContext(this);
		SetBackgroundStyle(wxBG_STYLE_CUSTOM);

		wxGLCanvas::SetCurrent(*glContext);
		GLenum err = glewInit();
		SPHE_ASSERT(err == GLEW_OK, "Error during GLEW initialization!");
	}

	ViewPort::~ViewPort()
	{
		delete glContext;
	}

	void ViewPort::OnPaintEvent(wxPaintEvent& evt)
	{
		wxPaintDC dc(this);
		Render(dc);
	}

	void ViewPort::PaintNow()
	{
		wxClientDC dc(this);
		Render(dc);
	}

	void ViewPort::Render(wxDC& dc)
	{
		wxGLCanvas::SetCurrent(*glContext);
		glClearColor(0.125f, 0.25f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glEnable(GL_TEXTURE_2D);   // textures
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		wxSize size = GetSize();
		glViewport(0, 0, size.x, size.x);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		gluOrtho2D(0, 0, size.x, size.x);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		// Render
		EditorData::GetLevel()->Render();

		glFlush();
		SwapBuffers();
	}
}
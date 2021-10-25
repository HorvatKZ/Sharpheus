#include "editor_pch.h"
#include "ViewPortBase.hpp"
#include "Editor/EditorCommands.hpp"


namespace Sharpheus {

	ViewPortBase::ViewPortBase(wxFrame* parent, const wxPoint& pos, const wxSize& size)
		: wxGLCanvas(parent, wxID_ANY, NULL, pos, size)
	{
		commonParent = new Collection(nullptr, "Root");
		camera = new ViewPortCamera(commonParent);
		camera->SetCustomRect(size.x, size.y);

		prevMousePos = wxGetMousePosition() - this->GetScreenPosition();

		Bind(wxEVT_PAINT, &ViewPortBase::OnPaintEvent, this);
	}


	ViewPortBase::~ViewPortBase()
	{
		delete commonParent;
		delete glContext;
	}


	void ViewPortBase::BindCallbacks(std::function<void()>&& currTrafoChangedCallback)
	{
		this->currTrafoChangedCallback = std::move(currTrafoChangedCallback);
	}


	void ViewPortBase::OnPaintEvent(wxPaintEvent& evt)
	{
		if (!EditorCommands::IsPlaying()) {
			wxPaintDC dc(this);
			Render(dc);
		}
	}


	void ViewPortBase::PaintNow()
	{
		if (!EditorCommands::IsPlaying()) {
			wxClientDC dc(this);
			Render(dc);
		}
	}


	void ViewPortBase::OnResize(wxSizeEvent& e)
	{
		wxSize size = GetSize();
		camera->SetCustomRect(size.x, size.y);
	}


	void ViewPortBase::OnScroll(wxMouseEvent& e)
	{
		wxPoint mousePos = wxGetMousePosition() - this->GetScreenPosition();
		camera->ZoomToScreen(e.GetWheelRotation() > 0.f ? 0.9f : 1.1f, Point(mousePos.x, mousePos.y));
		Refresh();
	}


	void ViewPortBase::OnMouseMove(wxMouseEvent& e)
	{
		wxPoint mousePos = wxGetMousePosition() - this->GetScreenPosition();
		wxPoint diff = mousePos - prevMousePos;
		if (e.MiddleIsDown()) {
			camera->MoveByScreen(Point(-diff.x, -diff.y));
			Refresh();
		}
	}

}
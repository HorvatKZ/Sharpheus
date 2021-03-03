#include "editor_pch.h"
#include "ViewPort.hpp"
#include "Editor/Registry/EditorData.hpp"
#include "Editor/Registry/ProjectData.hpp"


namespace Sharpheus {

	ViewPort::ViewPort(wxFrame* parent, const wxPoint& pos, const wxSize& size)
		: wxGLCanvas(parent, wxID_ANY, NULL, pos, size)
	{
		glContext = new wxGLContext(this);
		SetBackgroundStyle(wxBG_STYLE_CUSTOM);

		wxGLCanvas::SetCurrent(*glContext);
		GLenum err = glewInit();
		SPHE_ASSERT(err == GLEW_OK, "Error during GLEW initialization!");
		Renderer::Init();

		camera = new Camera(nullptr, "ViewPort camera");
		camera->SetCustomRect(size.x, size.y);

		prevMousePos = wxGetMousePosition() - this->GetScreenPosition();

		Bind(wxEVT_PAINT, &ViewPort::OnPaintEvent, this);
		Bind(wxEVT_SIZE, &ViewPort::OnResize, this);
		Bind(wxEVT_MOUSEWHEEL, &ViewPort::OnScroll, this);
		Bind(wxEVT_MOTION, &ViewPort::OnMouseMove, this);
		Bind(wxEVT_LEFT_DOWN, &ViewPort::OnMouseDown, this);
		Bind(wxEVT_LEFT_UP, &ViewPort::OnMouseUp, this);
	}


	ViewPort::~ViewPort()
	{
		delete glContext;
		delete camera;
		delete editArrow;
	}


	void ViewPort::BindCallbacks(std::function<void()>&& currChangedCallback, std::function<void()>&& currDataChangedCallback)
	{
		this->currChangedCallback = std::move(currChangedCallback);
		this->currDataChangedCallback = std::move(currDataChangedCallback);
	}


	void ViewPort::InitEditingArrow()
	{
		editArrow = new EditingArrow(camera);
	}


	void ViewPort::OnPaintEvent(wxPaintEvent& evt)
	{
		if (!isPlaying) {
			wxPaintDC dc(this);
			Render(dc);
		}
	}


	void ViewPort::PaintNow()
	{
		if (!isPlaying) {
			wxClientDC dc(this);
			Render(dc);
		}
	}


	void ViewPort::OnResize(wxSizeEvent& e)
	{
		wxSize size = GetSize();
		camera->SetCustomRect(size.x, size.y);
	}


	void ViewPort::OnScroll(wxMouseEvent& e)
	{
		wxPoint mousePos = wxGetMousePosition() - this->GetScreenPosition();
		camera->ZoomToScreen(e.GetWheelRotation() > 0.f ? 0.9f : 1.1f, Point(mousePos.x, mousePos.y));
		Refresh();
	}


	void ViewPort::OnMouseMove(wxMouseEvent& e)
	{
		wxPoint mousePos = wxGetMousePosition() - this->GetScreenPosition();
		wxPoint diff = mousePos - prevMousePos;
		if (e.MiddleIsDown()) {
			camera->MoveByScreen(Point(-diff.x, -diff.y));
			Refresh();
		} else if (e.LeftIsDown() && EditorData::GetCurrent() != nullptr) {
			GameObject* curr = EditorData::GetCurrent();
			Transform currTrafo = curr->GetWorldTrafo();
			Point camScale = camera->GetWorldTrafo().scale;
			switch (editType) {
				case EditingArrow::EditType::POS:
					{
						if (isGridOn && isSnapToGrid) {
							Point clickInGame = camera->ScreenPosToGamePos(Point(mousePos.x, mousePos.y));
							currTrafo.pos = Point(round(clickInGame.x / gridSize) * gridSize, round(clickInGame.y / gridSize) * gridSize);
						} else {
							currTrafo.pos += Point(diff.x * camScale.x, diff.y * camScale.y);
						}
					}
					break;
				case EditingArrow::EditType::ROT:
					{
						Point clickInGame = camera->ScreenPosToGamePos(Point(mousePos.x, mousePos.y));
						Point relativeClick = clickInGame - currTrafo.pos;
						float angle = relativeClick.GetAngle();
						if (relativeClick.y > 0) {
							angle *= -1;
						}
						angle += 135;
						if (angle > 180) {
							angle -= 360;
						}
						currTrafo.rot = angle;
					}
					break;
				case EditingArrow::EditType::SCALE:
					{
						Point prevScale = (camera->ScreenPosToGamePos(Point(prevMousePos.x, prevMousePos.y)) - currTrafo.pos).Rotate(-currTrafo.rot);
						Point currScale = (camera->ScreenPosToGamePos(Point(mousePos.x, mousePos.y)) - currTrafo.pos).Rotate(-currTrafo.rot);
						if (currScale.x != 0 && prevScale.x != 0) {
							currTrafo.scale.x *= currScale.x / prevScale.x;
						}
						if (currScale.y != 0 && prevScale.y != 0) {
							currTrafo.scale.y *= currScale.y / prevScale.y;
						}
					}
					break;
				case EditingArrow::EditType::POS_Y:
					{
						Point gameDiff = Point(diff.x * camScale.x, diff.y * camScale.y).Rotate(-currTrafo.rot);
						currTrafo.pos += Point(0, gameDiff.y).Rotate(currTrafo.rot);
					}
					break;
				case EditingArrow::EditType::POS_X:
					{
						Point gameDiff = Point(diff.x * camScale.x, diff.y * camScale.y).Rotate(-currTrafo.rot);
						currTrafo.pos += Point(gameDiff.x, 0).Rotate(currTrafo.rot);
					}
					break;
				case EditingArrow::EditType::SCALE_Y:
					{
						Point prevScale = (camera->ScreenPosToGamePos(Point(prevMousePos.x, prevMousePos.y)) - currTrafo.pos).Rotate(-currTrafo.rot);
						Point currScale = (camera->ScreenPosToGamePos(Point(mousePos.x, mousePos.y)) - currTrafo.pos).Rotate(-currTrafo.rot);
						if (currScale.y != 0 && prevScale.y != 0) {
							currTrafo.scale.y *= currScale.y / prevScale.y;
						}
					}
					break;
				case EditingArrow::EditType::SCALE_X:
					{
						Point prevScale = (camera->ScreenPosToGamePos(Point(prevMousePos.x, prevMousePos.y)) - currTrafo.pos).Rotate(-currTrafo.rot);
						Point currScale = (camera->ScreenPosToGamePos(Point(mousePos.x, mousePos.y)) - currTrafo.pos).Rotate(-currTrafo.rot);
						if (currScale.x != 0 && prevScale.x != 0) {
							currTrafo.scale.x *= currScale.x / prevScale.x;
						}
					}
					break;
			}

			if (editType != EditingArrow::EditType::NONE) {
				curr->SetWorldTrafo(currTrafo);
				currDataChangedCallback();
			}
		}
		prevMousePos = mousePos;
	}


	void ViewPort::OnMouseDown(wxMouseEvent& e)
	{
		wxPoint mousePos = wxGetMousePosition() - this->GetScreenPosition();
		if (EditorData::GetCurrent() != nullptr) {
			editType = editArrow->ClickOn(Point(mousePos.x, mousePos.y));
		}

		if (editType == EditingArrow::EditType::NONE) {
			Point gamePos = camera->ScreenPosToGamePos(Point(mousePos.x, mousePos.y));
			GameObject* selected = ProjectData::GetLevel()->GetRoot()->GetUpperMostSelected(gamePos);
			if (selected != nullptr) {
				EditorData::SetCurrent(selected);
				currChangedCallback();
			}
		}
	}


	void ViewPort::OnMouseUp(wxMouseEvent& e)
	{
		editType = EditingArrow::EditType::NONE;
	}


	void ViewPort::Render(wxDC& dc)
	{
		Camera* currCamera = Renderer::GetCamera();
		camera->SetCurrent();

		wxGLCanvas::SetCurrent(*glContext);
		Renderer::StartFrame();
		
		if (isGridOn && !isGridInForeground) {
			RenderGrid();
		}

		// Render
		ProjectData::GetLevel()->Render();

		if (isGridOn && isGridInForeground) {
			RenderGrid();
		}

		if (EditorData::GetCurrent() != nullptr) {
			EditorData::GetCurrent()->RenderAsSelected();
			Transform currTrafo = EditorData::GetCurrent()->GetWorldTrafo();
			editArrow->Render(Transform(Point(currTrafo.pos.x, currTrafo.pos.y), camera->GetWorldTrafo().scale, currTrafo.rot));
		}

		Renderer::EndFrame();
		SwapBuffers();

		currCamera->SetCurrent();
	}


	void ViewPort::RenderGrid()
	{
		camera->SetNeedToRecalc(true);
		float minX = camera->GetXMin();
		float maxX = camera->GetXMax();
		float minY = camera->GetYMin();
		float maxY = camera->GetYMax();

		float firstXGrid = ceil(minX / gridSize) * gridSize;
		float lastXGrid = floor(maxX / gridSize) * gridSize;
		float firstYGrid = ceil(minY / gridSize) * gridSize;
		float lastYGrid = floor(maxY / gridSize) * gridSize;

		float relativeXGridThickness = gridThickness * camera->GetWorldTrafo().scale.x / 2;
		float relativeYGridThickness = gridThickness * camera->GetWorldTrafo().scale.y / 2;
		while (firstXGrid <= lastXGrid) {
			Renderer::DrawMonocromeQuad(Point(firstXGrid - relativeXGridThickness, minY), Point(firstXGrid + relativeXGridThickness, minY),
				Point(firstXGrid + relativeXGridThickness, maxY), Point(firstXGrid - relativeXGridThickness, maxY), gridColor);
			firstXGrid += gridSize;
		}
		while (firstYGrid <= lastYGrid) {
			Renderer::DrawMonocromeQuad(Point(minX, firstYGrid - relativeYGridThickness), Point(maxX, firstYGrid - relativeYGridThickness),
				Point(maxX, firstYGrid + relativeYGridThickness), Point(minX, firstYGrid + relativeYGridThickness), gridColor);
			firstYGrid += gridSize;
		}
	}
}
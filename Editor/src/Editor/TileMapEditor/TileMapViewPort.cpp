#include "editor_pch.h"
#include "TileMapViewPort.hpp"
#include "Editor/Registry/EditorData.hpp"
#include "Editor/Registry/ProjectData.hpp"
#include "Editor/EditorWindow/EditorWindow.hpp"


namespace Sharpheus::TileMapEditor {

	ViewPort::ViewPort(wxFrame* parent, wxGLContext* other, const wxPoint& pos, const wxSize& size, TileSelector* selector, TileMap* tileMap)
		: ViewPortBase(parent, pos, size), selector(selector), tileMap(tileMap)
	{
		glContext = new wxGLContext(this, other);
		SetBackgroundStyle(wxBG_STYLE_CUSTOM);

		commonParent->SetWorldTrafo(tileMap->GetWorldTrafo());

		Bind(wxEVT_SIZE, &ViewPort::OnResize, this);
		Bind(wxEVT_MOUSEWHEEL, &ViewPort::OnScroll, this);
		Bind(wxEVT_MOTION, &ViewPort::OnMouseMove, this);
		Bind(wxEVT_LEFT_DOWN, &ViewPort::OnLeftDown, this);
		Bind(wxEVT_RIGHT_DOWN, &ViewPort::OnRightDown, this);
	}


	ViewPort::~ViewPort()
	{
	}


	void ViewPort::OnMouseMove(wxMouseEvent& e)
	{
		ViewPortBase::OnMouseMove(e);

		if (e.LeftIsDown()) {
			OnLeftDown(e);
		} else if (e.RightIsDown()) {
			OnRightDown(e);
		}

		prevMousePos = wxGetMousePosition() - this->GetScreenPosition();
	}


	void ViewPort::OnLeftDown(wxMouseEvent& e)
	{
		wxPoint mousePos = wxGetMousePosition() - this->GetScreenPosition();
		TileMap::IntPoint tileCoords = tileMap->ToTile(camera->ScreenPosToGamePos(Point(mousePos.x, mousePos.y)));
		byte newTile = selector->GetSelectedTile() + 1;
		if (tileMap->Get(tileCoords) != newTile) {
			tileMap->Set(tileCoords, newTile);
			((EditorWindow*)EditorData::GetMainWindow())->CurrentDataChanged();
		}
		Refresh();
	}


	void ViewPort::OnRightDown(wxMouseEvent& e)
	{
		wxPoint mousePos = wxGetMousePosition() - this->GetScreenPosition();
		TileMap::IntPoint tileCoords = tileMap->ToTile(camera->ScreenPosToGamePos(Point(mousePos.x, mousePos.y)));
		if (tileMap->Get(tileCoords) != 0) {
			tileMap->Clear(tileCoords);
			((EditorWindow*)EditorData::GetMainWindow())->CurrentDataChanged();
		}
		Refresh();
	}


	void ViewPort::Render(wxDC& dc)
	{
		if (commonParent->GetWorldTrafo() != tileMap->GetWorldTrafo()) {
			commonParent->SetWorldTrafo(tileMap->GetWorldTrafo());
		}

		Camera* currCamera = Renderer::GetCamera();
		camera->SetCurrent();

		wxGLCanvas::SetCurrent(*glContext);
		Renderer::StartFrame();

		ViewPortGridProps grid = EditorData::GetGridProps();
		if (isGridOn && !isForegroundOn) {
			RenderGrid();
		}

		// Render
		ProjectData::GetLevel()->Render();

		if (isGridOn && isForegroundOn) {
			RenderGrid();
		}

		if (isHighlightOn) {
			tileMap->RenderAsSelected();
		}
		
		Renderer::EndFrame();
		SwapBuffers();

		currCamera->SetCurrent();
	}


	void ViewPort::RenderGrid()
	{
		ViewPortGridProps grid = EditorData::GetGridProps();
		Point xAxis = tileMap->GetXAxis();
		Point yAxis = tileMap->GetYAxis();
		float halfCamWidth = camera->GetWidth() * 0.5f * camera->GetTrafo().scale.x;
		float halfCamHeight = camera->GetHeight() * 0.5f * camera->GetTrafo().scale.y;
		float relativeXGridThickness = grid.thickness * camera->GetWorldTrafo().scale.x;
		float relativeYGridThickness = grid.thickness * camera->GetWorldTrafo().scale.y;
		int32 width = tileMap->GetTileSet()->GetFrameWidth(), height = tileMap->GetTileSet()->GetFrameHeight();
		int32 xShift = camera->GetTrafo().pos.x / width, yShift = camera->GetTrafo().pos.y / height;
		Point startPos = (tileMap->GetWorldTrafo() + Transform(Point(xShift * width, yShift * height), camera->GetTrafo().scale, camera->GetTrafo().rot)).pos;

		int32 firstX = -halfCamWidth / width - 1, lastX = -firstX;
		int32 firstY = -halfCamHeight / height - 1, lastY = -firstY;

		while (firstX <= lastX) {
			Point center = startPos + xAxis * firstX * width;
			Point diff = yAxis * halfCamHeight * (1.f + height / camera->GetTrafo().scale.x);
			float mult = ((firstX + xShift) % 16 == 0) ? 2.f : 1.f;
			Renderer::DrawLine(center - diff, center + diff, mult * relativeXGridThickness, grid.color);
			++firstX;
		}
		while (firstY <= lastY) {
			Point center = startPos + yAxis * firstY * height;
			Point diff = xAxis * halfCamWidth * (1.f + width / camera->GetTrafo().scale.y);
			float mult = ((firstY + yShift) % 16 == 0) ? 2.f : 1.f;
			Renderer::DrawLine(center - diff, center + diff, mult * relativeYGridThickness, grid.color);
			++firstY;
		}
	}
}
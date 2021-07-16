#include "editor_pch.h"
#include "TileMapToolBar.hpp"
#include "TileMapViewPort.hpp"
#include "Editor/ResourceManagement/ImageManager.hpp"


namespace Sharpheus::TileMapEditor {

	ToolBar::ToolBar(wxFrame* parent, const wxPoint& pos, const wxSize& size, ViewPort* viewPort)
		: wxPanel(parent, wxID_ANY, pos, size), viewPort(viewPort)
	{
		InitBitmaps();

		refreshButton = new wxBitmapButton(this, wxID_ANY, refresh, wxPoint(8, 8), wxSize(24, 24));
		hr = new wxStaticLine(this, wxID_ANY, wxPoint(3, 40), wxSize(34, 1));
		gridButton = new wxBitmapButton(this, wxID_ANY, gridOn, wxPoint(8, 49), wxSize(24, 24));
		foregroundButton = new wxBitmapButton(this, wxID_ANY, foregroundOn, wxPoint(8, 76), wxSize(24, 24));
		highlightButton = new wxBitmapButton(this, wxID_ANY, highlightOff, wxPoint(8, 103), wxSize(24, 24));

		refreshButton->Bind(wxEVT_BUTTON, &ToolBar::OnRefreshPressed, this);
		gridButton->Bind(wxEVT_BUTTON, &ToolBar::OnGridPressed, this);
		foregroundButton->Bind(wxEVT_BUTTON, &ToolBar::OnForegroundPressed, this);
		highlightButton->Bind(wxEVT_BUTTON, &ToolBar::OnHighlightPressed, this);
	}


	ToolBar::~ToolBar()
	{
	}


	void ToolBar::InitBitmaps()
	{
		refresh = ImageManager::GetImage("refresh.png", ImageManager::PathType::TOOLBAR);
		gridOn = ImageManager::GetImage("grid_on.png", ImageManager::PathType::TOOLBAR);
		gridOff = ImageManager::GetImage("grid_off.png", ImageManager::PathType::TOOLBAR);
		foregroundOn = ImageManager::GetImage("foreground_on.png", ImageManager::PathType::TOOLBAR);
		foregroundOff = ImageManager::GetImage("foreground_off.png", ImageManager::PathType::TOOLBAR);
		highlightOn = ImageManager::GetImage("highlight_on.png", ImageManager::PathType::TOOLBAR);
		highlightOff = ImageManager::GetImage("highlight_off.png", ImageManager::PathType::TOOLBAR);
	}


	void ToolBar::OnRefreshPressed(wxCommandEvent& e)
	{
		viewPort->Refresh();
	}


	void ToolBar::OnGridPressed(wxCommandEvent& e)
	{
		if (viewPort->IsGridOn()) {
			viewPort->SetGrid(false);
			gridButton->SetBitmap(gridOff);
			foregroundButton->Enable(false);
		}
		else {
			viewPort->SetGrid(true);
			gridButton->SetBitmap(gridOn);
			foregroundButton->Enable(true);
		}
		viewPort->Refresh();
	}


	void ToolBar::OnForegroundPressed(wxCommandEvent& e)
	{
		if (viewPort->IsForegroundOn()) {
			viewPort->SetForeground(false);
			foregroundButton->SetBitmap(foregroundOff);
		}
		else {
			viewPort->SetForeground(true);
			foregroundButton->SetBitmap(foregroundOn);
		}
		viewPort->Refresh();
	}


	void ToolBar::OnHighlightPressed(wxCommandEvent& e)
	{
		if (viewPort->IsHighlightOn()) {
			viewPort->SetHighlight(false);
			highlightButton->SetBitmap(highlightOff);
		}
		else {
			viewPort->SetHighlight(true);
			highlightButton->SetBitmap(highlightOn);
		}
		viewPort->Refresh();
	}


}
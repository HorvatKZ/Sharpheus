#include "editor_pch.h"
#include "TileSelecor.hpp"
#include "Editor/ResourceManagement/ImageManager.hpp"


namespace Sharpheus::TileMapEditor {

	const uint32_t TileSelector::tileSize = 100;
	const uint32_t TileSelector::border = 10;
	const wxColour TileSelector::selectColor(240, 180, 20);


	TileSelector::TileSelector(wxFrame* parent, const wxPoint& pos, const wxSize& size)
		: wxScrolledWindow(parent, wxID_ANY, pos, size)
	{
		scrollHeight = GetClientSize().y;

		Bind(wxEVT_PAINT, &TileSelector::OnPaintEvent, this);
		Bind(wxEVT_LEFT_DOWN, &TileSelector::OnClick, this);
		Bind(wxEVT_SIZE, &TileSelector::OnResize, this);
	}


	TileSelector::~TileSelector()
	{
	}


	void TileSelector::SetTileSet(TileSet* tileSet)
	{
		if (tileSet != nullptr && !tileSet->IsValid()) {
			SPHE_ERROR("TileSelector: Invalid TileSet");
		}

		this->tileSet = tileSet;
		atlas = ImageManager::GetImage(tileSet->GetAtlas()->GetFullPath());
		tileImgs.clear();
		for (uint32_t i = 0; i < tileSet->GetNumOfTiles(); ++i) {
			tileImgs.push_back(GetTile(i));
		}
		Refresh();
	}


	void TileSelector::OnPaintEvent(wxPaintEvent& evt)
	{
		wxPaintDC dc(this);
		Draw(dc);
	}


	void TileSelector::PaintNow()
	{
		wxClientDC dc(this);
		Draw(dc);
	}


	void TileSelector::OnClick(wxMouseEvent& e)
	{
		wxPoint pos = e.GetPosition() + GetViewStart() * UI::scrollSpeed;
		uint32_t col = pos.x / (border + tileSize);
		uint32_t row = pos.y / (border + tileSize + 20);
		uint32_t perRow = (GetClientSize().x - border) / (border + tileSize);

		if (col >= perRow) {
			return;
		}

		uint32_t ind = row * perRow + col;
		if (ind < tileSet->GetNumOfTiles()) {
			selected = ind;
			Refresh();
		}
	}


	void TileSelector::OnResize(wxSizeEvent& e)
	{
		Refresh();
	}


	void TileSelector::Draw(wxClientDC& dc)
	{
		PrepareDC(dc);
		dc.Clear();

		uint32_t perRow = (GetClientSize().x - border) / (border + tileSize);
		for (uint32_t i = 0; i < tileSet->GetNumOfTiles(); ++i) {
			DrawTile(dc, i, i % perRow, i / perRow);
		}

		uint32_t heightNeeded = ((tileSet->GetNumOfTiles() - 1) / perRow + 1) * (tileSize + 20 + border);
		uint32_t realHeight = GetClientSize().y;
		if (heightNeeded < realHeight && scrollHeight != realHeight) {
			scrollHeight = realHeight;
			SetScrollbars(0, UI::scrollSpeed, GetClientSize().x, scrollHeight / UI::scrollSpeed);
		}
		else if (heightNeeded > realHeight && heightNeeded != scrollHeight) {
			scrollHeight = heightNeeded;
			SetScrollbars(0, UI::scrollSpeed, GetClientSize().x, scrollHeight / UI::scrollSpeed);
		}
	}


	void TileSelector::DrawTile(wxClientDC& dc, uint32_t ind, uint32_t i, uint32_t j)
	{
		if (ind == selected) {
			dc.SetBrush(selectColor);
			dc.DrawRectangle(wxPoint(i * (tileSize + border) + border - 3, j * (tileSize + 20 + border) + border - 3), wxSize(tileSize + 6, tileSize + 25));
		}

		dc.DrawBitmap(tileImgs[ind], wxPoint(i * (tileSize + border) + border, j * (tileSize + 20 + border) + border));

		wxString indStr = wxString::Format("%d", ind);
		uint32_t extent = GetTextExtent(indStr).x;
		dc.DrawLabel(indStr, wxRect(i * (tileSize + border) + border + (tileSize - extent) / 2, j * (tileSize + 20 + border) + border + tileSize, extent, UI::unitHeight));
	}


	wxBitmap TileSelector::GetTile(uint32_t ind)
	{
		uint32_t currCol = ind % tileSet->GetFrameCols(), currRow = ind / tileSet->GetFrameCols();
		uint32_t frameWidth = tileSet->GetFrameWidth(), frameHeight = tileSet->GetFrameHeight();
		wxImage bitmap = atlas.GetSubImage(wxRect(wxPoint(frameWidth * currCol, frameHeight * currRow), wxSize(frameWidth, frameHeight)));
		wxSize newBitmapSize;
		if (frameWidth > frameHeight) {
			newBitmapSize = wxSize(tileSize, frameHeight * tileSize / frameWidth);
		} else {
			newBitmapSize = wxSize(frameWidth * tileSize / frameHeight, tileSize);
		}
		bitmap.Rescale(newBitmapSize.x, newBitmapSize.y);
		return bitmap;
	}

}
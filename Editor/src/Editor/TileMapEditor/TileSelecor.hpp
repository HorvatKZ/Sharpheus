#pragma once

#include "editor_pch.h"
#include <wx/scrolwin.h>


namespace Sharpheus::TileMapEditor {

	class TileSelector : public wxScrolledWindow
	{
	public:
		TileSelector(wxFrame* parent, const wxPoint& pos, const wxSize& size);
		virtual ~TileSelector();

		void SetTileSet(TileSet* tileSet);

		void OnPaintEvent(wxPaintEvent& evt);
		void PaintNow();

		inline uint8_t GetSelectedTile() { return selected; }

	private:
		TileSet* tileSet = nullptr;
		wxImage atlas;
		std::vector<wxBitmap> tileImgs;
		uint32_t scrollHeight;
		uint8_t selected = 0;

		static const uint32_t tileSize;
		static const uint32_t border;
		static const wxColour selectColor;

		void OnClick(wxMouseEvent& e);
		void OnResize(wxSizeEvent& e);

		void Draw(wxClientDC& dc);
		void DrawTile(wxClientDC& dc, uint32_t ind, uint32_t i, uint32_t j);

		wxBitmap GetTile(uint32_t ind);
	};

}
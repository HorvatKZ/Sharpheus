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

		inline byte GetSelectedTile() { return selected; }

	private:
		TileSet* tileSet = nullptr;
		wxImage atlas;
		std::vector<wxBitmap> tileImgs;
		uint32 scrollHeight;
		byte selected = 0;

		static const uint32 tileSize;
		static const uint32 border;
		static const wxColour selectColor;

		void OnClick(wxMouseEvent& e);
		void OnResize(wxSizeEvent& e);

		void Draw(wxClientDC& dc);
		void DrawTile(wxClientDC& dc, uint32 ind, uint32 i, uint32 j);

		wxBitmap GetTile(uint32 ind);
	};

}
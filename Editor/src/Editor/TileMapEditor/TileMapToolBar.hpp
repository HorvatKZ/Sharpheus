#pragma once

#include "editor_pch.h"
#include <wx/statline.h>


namespace Sharpheus::TileMapEditor {

	class ToolBar : public wxPanel
	{
	public:
		ToolBar(wxFrame* parent, const wxPoint& pos, const wxSize& size, class ViewPort* viewPort);
		virtual ~ToolBar();

	private:
		wxBitmapButton* refreshButton;
		wxStaticLine* hr;
		wxBitmapButton* gridButton;
		wxBitmapButton* foregroundButton;
		wxBitmapButton* highlightButton;

		wxBitmap refresh, gridOn, gridOff, foregroundOn, foregroundOff, highlightOn, highlightOff;
		ViewPort* viewPort;

		void InitBitmaps();
		void OnRefreshPressed(wxCommandEvent& e);
		void OnGridPressed(wxCommandEvent& e);
		void OnForegroundPressed(wxCommandEvent& e);
		void OnHighlightPressed(wxCommandEvent& e);
	};

}
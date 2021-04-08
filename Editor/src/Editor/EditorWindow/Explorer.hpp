#pragma once

#include "editor_pch.h"
#include <wx/scrolwin.h>
#include <wx/filename.h>


namespace Sharpheus {

	class Explorer : public wxScrolledWindow
	{
	public:
		Explorer(wxFrame* parent, const wxPoint& pos, const wxSize& size);
		virtual ~Explorer();

		void SetPath(const wxString& path);

		void OnPaintEvent(wxPaintEvent& evt);
		void PaintNow();

	private:
		wxString basePath;
		wxFileName folder;
		wxBitmap backFolderImg, folderImg, cImg, hImg, cppImg, hppImg, txtImg, logImg, imgImg, animImg, lvlImg, projImg, unknownImg;
		static uint32_t oneWidth;

		void OnDoubleClick(wxMouseEvent& e);
		void OnResize(wxSizeEvent& e);

		void Draw(wxClientDC& dc);
		void DrawIcon(wxClientDC& dc, const wxBitmap& img, uint32_t i, uint32_t j);
		void DrawText(wxClientDC& dc, const wxString& text, uint32_t i, uint32_t j);
	};

}
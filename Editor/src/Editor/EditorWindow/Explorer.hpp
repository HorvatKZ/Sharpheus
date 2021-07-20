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
		wxBitmap backFolderImg, folderImg, unknownImg;
		std::unordered_map<wxString, wxBitmap> imgs;
		uint32 scrollHeight;
		static const uint32 oneWidth;

		void OnDoubleClick(wxMouseEvent& e);
		void OnResize(wxSizeEvent& e);

		void Draw(wxClientDC& dc);
		void DrawIcon(wxClientDC& dc, const wxBitmap& img, uint32 i, uint32 j);
		void DrawText(wxClientDC& dc, const wxString& text, uint32 i, uint32 j);
	};

}
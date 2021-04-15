#include "editor_pch.h"
#include "Explorer.hpp"
#include "Editor/ResourceManagement/ImageManager.hpp"
#include <wx/dir.h>


namespace Sharpheus {

	uint32_t Explorer::oneWidth = 120;


	Explorer::Explorer(wxFrame* parent, const wxPoint& pos, const wxSize& size)
		: wxScrolledWindow(parent, wxID_ANY, pos, size),
		backFolderImg	(ImageManager::GetImage("backfolder.png", ImageManager::PathType::EXPLORER)),
		folderImg		(ImageManager::GetImage("folder.png", ImageManager::PathType::EXPLORER)),
		cImg			(ImageManager::GetImage("c.png", ImageManager::PathType::EXPLORER)),
		hImg			(ImageManager::GetImage("h.png", ImageManager::PathType::EXPLORER)),
		cppImg			(ImageManager::GetImage("cpp.png", ImageManager::PathType::EXPLORER)),
		hppImg			(ImageManager::GetImage("hpp.png", ImageManager::PathType::EXPLORER)),
		txtImg			(ImageManager::GetImage("txt.png", ImageManager::PathType::EXPLORER)),
		logImg			(ImageManager::GetImage("log.png", ImageManager::PathType::EXPLORER)),
		imgImg			(ImageManager::GetImage("img.png", ImageManager::PathType::EXPLORER)),
		animImg			(ImageManager::GetImage("anim.png", ImageManager::PathType::EXPLORER)),
		lvlImg			(ImageManager::GetImage("lvl.png", ImageManager::PathType::EXPLORER)),
		projImg			(ImageManager::GetImage("proj.png", ImageManager::PathType::EXPLORER)),
		unknownImg		(ImageManager::GetImage("unknown.png", ImageManager::PathType::EXPLORER))
	{
		scrollHeight = GetClientSize().y;

		Bind(wxEVT_PAINT, &Explorer::OnPaintEvent, this);
		Bind(wxEVT_LEFT_DCLICK, &Explorer::OnDoubleClick, this);
		Bind(wxEVT_SIZE, &Explorer::OnResize, this);
	}


	Explorer::~Explorer()
	{
	}


	void Explorer::SetPath(const wxString& path)
	{
		basePath = path;
		folder = wxFileName::DirName(path);
		Refresh();
	}


	void Explorer::OnPaintEvent(wxPaintEvent& evt)
	{
		wxPaintDC dc(this);
		Draw(dc);
	}


	void Explorer::PaintNow()
	{
		wxClientDC dc(this);
		Draw(dc);
	}


	void Explorer::OnDoubleClick(wxMouseEvent& e)
	{
		wxPoint pos = e.GetPosition() + GetViewStart();
		uint32_t col = pos.x / (UI::border + oneWidth);
		uint32_t row = pos.y / (UI::border + 120);
		uint32_t perRow = (GetClientSize().x - UI::border) / (UI::border + oneWidth);

		if (col >= perRow) {
			return;
		}

		uint32_t ind = row * perRow + col;
		wxString name, folderPath = folder.GetPath();
		if (folderPath.Last() != '\\') {
			folderPath += '\\';
		}
		if (folderPath != basePath) {
			if (ind == 0) {
				folder.RemoveLastDir();
				Refresh();
				return;
			}
			--ind;
		}

		wxDir dir(folderPath);
		dir.GetFirst(&name);
		for (int i = 0; i < ind; ++i) {
			dir.GetNext(&name);
		}

		if (wxFileName::DirExists(folderPath + name)) {
			folder.AppendDir(name);
			Refresh();
		}
		else {
			wxMessageBox("Cannot open " + name, "Error", wxICON_ERROR | wxOK | wxCENTRE);
		}
	}


	void Explorer::OnResize(wxSizeEvent& e)
	{
		Refresh();
	}


	void Explorer::Draw(wxClientDC& dc)
	{
		PrepareDC(dc);
		dc.Clear();

		uint32_t i = 0;
		wxString folderPath = folder.GetFullPath();
		if (folderPath.Last() != '\\') {
			folderPath += '\\';
		}
		if (folderPath != basePath) {
			DrawIcon(dc, backFolderImg, 0, 0);
			DrawText(dc, "..", 0, 0);
			++i;
		}

		wxString name;
		wxDir dir(folderPath);
		bool found = dir.GetFirst(&name);

		uint32_t perRow = (GetClientSize().x - UI::border) / (UI::border + oneWidth);
		while (found) {
			wxString displayName = name;
			uint32_t col = i % perRow, row = i / perRow;
			if (wxFileName::DirExists(folderPath + name)) {
				DrawIcon(dc, folderImg, col, row);
			}
			else {
				wxString ext = name.Mid(name.find_last_of('.') + 1);
				if (ext == "sharpheus") {
					wxString withoutExt = name.Left(name.Length() - ext.Length() - 1);
					wxString subExt = withoutExt.Mid(withoutExt.find_last_of('.') + 1);
					displayName = withoutExt.Left(withoutExt.Length() - subExt.Length() - 1);
					
					if (subExt == "lvl") {
						DrawIcon(dc, lvlImg, col, row);
					}
					else if (subExt == "anim") {
						DrawIcon(dc, animImg, col, row);
					}
					else if (subExt == "proj") {
						DrawIcon(dc, projImg, col, row);
					}
					else {
						DrawIcon(dc, unknownImg, col, row);
					}
				}
				else if (ext == "png" || ext == "jpg" || ext == "jpeg" || ext == "bmp" || ext == "gif") {
					DrawIcon(dc, imgImg, col, row);
				}
				else if (ext == "cpp") {
					DrawIcon(dc, cppImg, col, row);
				}
				else if (ext == "hpp") {
					DrawIcon(dc, hppImg, col, row);
				}
				else if (ext == "h") {
					DrawIcon(dc, hImg, col, row);
				}
				else if (ext == "c") {
					DrawIcon(dc, cImg, col, row);
				}
				else if (ext == "txt") {
					DrawIcon(dc, txtImg, col, row);
				}
				else if (ext == "log") {
					DrawIcon(dc, logImg, col, row);
				}
				else {
					DrawIcon(dc, unknownImg, col, row);
				}
			}

			DrawText(dc, displayName, col, row);
			found = dir.GetNext(&name);
			++i;
		}

		uint32_t heightNeeded = i / perRow * (120 + UI::border);
		uint32_t realHeight = GetClientSize().y;
		if (heightNeeded < realHeight && scrollHeight != realHeight) {
			scrollHeight = realHeight;
			SetScrollbars(1, 1, GetClientSize().x, scrollHeight);
		}
		else if (heightNeeded > realHeight && heightNeeded != scrollHeight) {
			scrollHeight = heightNeeded;
			SetScrollbars(1, 1, GetClientSize().x, scrollHeight);
		}
	}


	void Explorer::DrawIcon(wxClientDC& dc, const wxBitmap& img, uint32_t i, uint32_t j)
	{
		dc.DrawBitmap(img, wxPoint(i * (oneWidth + UI::border) + UI::border + (oneWidth - 80) / 2, j * (120 + UI::border) + UI::border));
	}


	void Explorer::DrawText(wxClientDC& dc, const wxString& text, uint32_t i, uint32_t j)
	{
		uint32_t extent = GetTextExtent(text).x;
		dc.DrawLabel(text, wxRect(i * (oneWidth + UI::border) + UI::border + (oneWidth - extent) / 2, j * (120 + UI::border) + UI::border + 80, extent, 22));
	}

}
#include "editor_pch.h"
#include "Explorer.hpp"
#include "Editor/ResourceManagement/ImageManager.hpp"
#include "Editor/EditorCommands.hpp"
#include <wx/dir.h>


namespace Sharpheus {

	uint32_t Explorer::oneWidth = 120;


	Explorer::Explorer(wxFrame* parent, const wxPoint& pos, const wxSize& size)
		: wxScrolledWindow(parent, wxID_ANY, pos, size),
		backFolderImg	(ImageManager::GetImage("backfolder.png", ImageManager::PathType::EXPLORER)),
		folderImg		(ImageManager::GetImage("folder.png", ImageManager::PathType::EXPLORER)),
		unknownImg		(ImageManager::GetImage("unknown.png", ImageManager::PathType::EXPLORER))
	{
		scrollHeight = GetClientSize().y;

		imgs["c"] = ImageManager::GetImage("c.png", ImageManager::PathType::EXPLORER);
		imgs["h"] = ImageManager::GetImage("h.png", ImageManager::PathType::EXPLORER);
		imgs["cpp"] = ImageManager::GetImage("cpp.png", ImageManager::PathType::EXPLORER);
		imgs["hpp"] = ImageManager::GetImage("hpp.png", ImageManager::PathType::EXPLORER);
		imgs["txt"] = ImageManager::GetImage("txt.png", ImageManager::PathType::EXPLORER);
		imgs["log"] = ImageManager::GetImage("log.png", ImageManager::PathType::EXPLORER);
		imgs["img"] = ImageManager::GetImage("img.png", ImageManager::PathType::EXPLORER);
		imgs["anim"] = ImageManager::GetImage("anim.png", ImageManager::PathType::EXPLORER);
		imgs["tile"] = ImageManager::GetImage("tile.png", ImageManager::PathType::EXPLORER);
		imgs["scene"] = ImageManager::GetImage("scene.png", ImageManager::PathType::EXPLORER);
		imgs["lvl"] = ImageManager::GetImage("lvl.png", ImageManager::PathType::EXPLORER);
		imgs["proj"] = ImageManager::GetImage("proj.png", ImageManager::PathType::EXPLORER);

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
		wxPoint pos = e.GetPosition() + GetViewStart() * UI::scrollSpeed;
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
			return;
		}

		wxString ext = name.Mid(name.find_last_of('.') + 1);
		if (ext == "sharpheus") {
			wxString withoutExt = name.Left(name.Length() - ext.Length() - 1);
			wxString subExt = withoutExt.Mid(withoutExt.find_last_of('.') + 1);
			if (subExt == "proj") {
				wxMessageBox(name + " is the currently open project file", "Info");
				return;
			}
			else if (subExt == "lvl") {
				int response = wxMessageBox("Are you sure you want to open level: " + name, "Info", wxICON_INFORMATION | wxYES | wxNO | wxCENTRE);
				if (response == wxNO) {
					return;
				}

				wxString relativePath = folderPath.Mid(basePath.Length()) + name;
				if (relativePath.Length() < 7 || relativePath.Left(7) != "Levels\\") {
					wxMessageBox("Level: " + name + " is not under the Levels folder", "Error", wxICON_ERROR | wxOK | wxCENTRE);
					return;
				}

				wxString levelPath = relativePath.Mid(7);
				EditorCommands::LoadLevel(levelPath);
				return;
			}
			else if (subExt == "scene") {
				wxString relativePath = folderPath.Mid(basePath.Length()) + name;
				if (relativePath.Length() < 7 || relativePath.Left(7) != "Scenes\\") {
					wxMessageBox("Scene: " + name + " is not under the Scenes folder", "Error", wxICON_ERROR | wxOK | wxCENTRE);
					return;
				}

				wxString scenePath = relativePath.Mid(7);
				EditorCommands::AttachSceneToCurrent(scenePath);
				return;
			}
			else if (subExt == "anim") {
				wxString relativePath = folderPath.Mid(basePath.Length()) + name;
				if (relativePath.Length() < 7 || relativePath.Left(7) != "Assets\\") {
					wxMessageBox("Animation: " + name + " is not under the Assets folder", "Error", wxICON_ERROR | wxOK | wxCENTRE);
					return;
				}

				wxString animPath = relativePath.Mid(7);
				EditorCommands::EditAnimation(animPath);
				return;
			}
			else if (subExt == "tile") {
				wxString relativePath = folderPath.Mid(basePath.Length()) + name;
				if (relativePath.Length() < 7 || relativePath.Left(7) != "Assets\\") {
					wxMessageBox("TileSet: " + name + " is not under the Assets folder", "Error", wxICON_ERROR | wxOK | wxCENTRE);
					return;
				}

				wxString tileSetPath = relativePath.Mid(7);
				EditorCommands::EditTileSet(tileSetPath);
				return;
			}
		}
		wxMessageBox(name + " is not a *.sharpheus file", "Error", wxICON_ERROR | wxOK | wxCENTRE);
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
				if (ImageManager::IsSupportedImgFormat(ext)) {
					DrawIcon(dc, imgs["img"], col, row);
				}
				else {
					if (ext == "sharpheus") {
						wxString withoutExt = name.Left(name.Length() - ext.Length() - 1);
						ext = withoutExt.Mid(withoutExt.find_last_of('.') + 1);
						displayName = withoutExt.Left(withoutExt.Length() - ext.Length() - 1);
					}

					auto it = imgs.find(ext);
					if (it != imgs.end()) {
						DrawIcon(dc, (*it).second, col, row);
					} else {
						DrawIcon(dc, unknownImg, col, row);
					}
				}
			}

			DrawText(dc, displayName, col, row);
			found = dir.GetNext(&name);
			++i;
		}

		uint32_t heightNeeded = ((i - 1) / perRow + 1) * (120 + UI::border);
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


	void Explorer::DrawIcon(wxClientDC& dc, const wxBitmap& img, uint32_t i, uint32_t j)
	{
		dc.DrawBitmap(img, wxPoint(i * (oneWidth + UI::border) + UI::border + (oneWidth - 80) / 2, j * (120 + UI::border) + UI::border));
	}


	void Explorer::DrawText(wxClientDC& dc, const wxString& text, uint32_t i, uint32_t j)
	{
		uint32_t extent = GetTextExtent(text).x;
		dc.DrawLabel(text, wxRect(i * (oneWidth + UI::border) + UI::border + (oneWidth - extent) / 2, j * (120 + UI::border) + UI::border + 80, extent, UI::unitHeight));
	}

}
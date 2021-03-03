#include "editor_pch.h"
#include "ImageManager.hpp"
#include "Editor/Registry/EditorData.hpp"


namespace Sharpheus {

	void ImageManager::Init()
	{
		wxInitAllImageHandlers();
	}

	void ImageManager::Clear()
	{
	}

	wxImage ImageManager::GetImage(const wxString& path, PathType type)
	{
		wxBitmapType format = wxBITMAP_TYPE_INVALID;
		wxString formatString = path.SubString(path.find_last_of(".") + 1, path.Length() - 1);
		if (formatString == "png") {
			format = wxBITMAP_TYPE_PNG;
		} else if (formatString == "jpg" || formatString == "jpeg") {
			format = wxBITMAP_TYPE_JPEG;
		} else if (formatString == "bmp") {
			format = wxBITMAP_TYPE_JPEG;
		} else if (formatString == "gif") {
			format = wxBITMAP_TYPE_GIF;
		}

		wxString fullPath;
		switch (type) {
			case PathType::RELATIVE_PATH:
				fullPath = EditorData::GetPath() + "Assets\\Editor\\" + path;
				break;
			case PathType::DETAILSPANEL:
				fullPath = EditorData::GetPath() + "Assets\\Editor\\Icons\\DetailsPanel\\" + path;
				break;
			case PathType::TOOLBAR:
				fullPath = EditorData::GetPath() + "Assets\\Editor\\Icons\\ToolBar\\" + path;
				break;
			case PathType::GAMEOBJECTS:
				fullPath = EditorData::GetPath() + "Assets\\Editor\\Icons\\GameObjects\\" + path;
				break;
			default:
				fullPath = path;
		}

		return wxImage(fullPath, format);
	}
}
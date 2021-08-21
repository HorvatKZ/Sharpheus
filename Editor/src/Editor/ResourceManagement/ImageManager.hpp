#pragma once

#include <wx/wx.h>


namespace Sharpheus {

	class ImageManager
	{
	public:
		enum class PathType
		{
			ABSOLUTE_PATH, RELATIVE_PATH,
			DETAILSPANEL, GAMEOBJECTS, TOOLBAR, EXPLORER, MENUBAR
		};

		static void Init();
		static void Clear();

		static wxImage GetImage(const wxString& path, PathType type = PathType::ABSOLUTE_PATH);

		static bool IsSupportedImgFormat(const wxString& format);
	};

}
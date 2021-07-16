#pragma once

#include "editor_pch.h"
#include <wx/glcanvas.h>

namespace Sharpheus {

	class EditorData
	{
	public:
		struct GridProps {
			float size = 64, thickness = 2, camOutlineThickness = 5;
			bool isInForeground = true, isCamOutlineVisible = true;
			Color color = Color(240, 240, 240, 200);
			Color camOutlineColor = Color(255, 32, 64, 200);
		};

		static void Init(const wxString& configFile);
		static void Clear();

		static inline class GameObject* GetCurrent() { return curr; }
		static inline const wxString& GetPath() { return path; }
		static void SetCurrent(const wxString& currName);
		static void SetCurrent(class GameObject* _curr);

		static inline const GridProps& GetGridProps() { return gridProps; }
		static inline void SetGridProps(const GridProps& grid) { gridProps = grid; }

		static inline const wxString& GetVersion() { return version; }
		static inline const wxString& GetOGLVersion() { return oglVersion; }
		static inline void SetOGLVersion(const wxString& newOglVersion) { oglVersion = newOglVersion; }

		static inline wxFrame* GetMainWindow() { return editorWindow; }
		static inline void SetMainWindow(wxFrame* window) { editorWindow = window; }

		static inline wxGLContext* GetMainContext() { return mainContext; }
		static inline void SetMainContext(wxGLContext* context) { mainContext = context; }
	
	private:
		static class GameObject* curr;
		static wxString path;
		static wxString configFile;
		static GridProps gridProps;
		static wxString version;
		static wxString oglVersion;
		static wxFrame* editorWindow;
		static wxGLContext* mainContext;
	};

	typedef EditorData::GridProps ViewPortGridProps;
}
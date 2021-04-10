#pragma once


namespace Sharpheus {

	class EditorData
	{
	public:
		struct GridProps {
			float size = 64, thickness = 2;
			bool isInForeground = true;
			Color color = Color(240, 240, 240, 200);
		};

		static void Init(const wxString& configFile);
		static void Clear();

		static inline class GameObject* GetCurrent() { return curr; }
		static inline const wxString& GetPath() { return path; }
		static void SetCurrent(const wxString& currName);
		static void SetCurrent(class GameObject* _curr);

		static inline const GridProps& GetGridProps() { return gridProps; }
		static inline void SetGridProps(const GridProps& grid) { gridProps = grid; }
	
	private:
		static class GameObject* curr;
		static wxString path;
		static wxString configFile;
		static GridProps gridProps;
	};

	typedef EditorData::GridProps ViewPortGridProps;
}
#pragma once


namespace Sharpheus {

	class EditorData
	{
	public:
		static void Init(const wxString& configFile);
		static void Clear();

		static inline class GameObject* GetCurrent() { return curr; }
		static inline const wxString& GetPath() { return path; }
		static void SetCurrent(const wxString& currName);
		static void SetCurrent(class GameObject* _curr);
	
	private:
		static class GameObject* curr;
		static wxString path;
	};

}
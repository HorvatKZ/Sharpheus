#pragma once


namespace Sharpheus {

	class EditorData
	{
	public:
		static void Init();
		static void Clear();

		static inline class GameObject* GetCurrent() { return curr; }
		static inline const std::string& GetPath() { return path; }
		static void SetCurrent(const std::string& currName);
		static void SetCurrent(class GameObject* _curr);
	
	private:
		static class GameObject* curr;
		static std::string path;
	};

}